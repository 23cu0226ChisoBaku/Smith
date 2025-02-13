// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnActor_Test.h"
#include "AttackHandle.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithTurnBaseAIAttackStrategy.h"
#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithTurnBaseAIIdleStrategy.h"
#include "SmithAttackComponent.h"
#include "SmithMoveComponent.h"

#include "SmithAnimationComponent.h"

#include "FormatInfo_Import.h"
#include "SmithMoveDirector.h"
#include "Direction.h"
#include "SmithPickable.h"
#include "IEventPublishMediator.h"
#include "SmithBattleLogWorldSubsystem.h"
#include "MLibrary.h"

#include "SmithEnemyParamInitializer.h"
#include "SmithEnemyLootGenerator.h"

#include "BattleParamHandle.h"

ATurnActor_Test::ATurnActor_Test()
	: m_attackStrategy(nullptr)
	, m_moveStrategy(nullptr)
	, m_idleStrategy(nullptr)
	, AtkComponent(nullptr)
	, MoveComponent(nullptr)
	, AnimComponent(nullptr)
	, m_level(1)
	, m_bIsPlayingDeadAnimation(false)
{
	PrimaryActorTick.bCanEverTick = true;
	SetTurnPriority(ETurnPriority::Rival);

	AtkComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("attack comp test"));
	check(AtkComponent != nullptr);

	MoveComponent = CreateDefaultSubobject<USmithMoveComponent>(TEXT("move comp test"));
	check(MoveComponent != nullptr);

	AnimComponent = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("anim comp"));
	check(AnimComponent != nullptr)

}

void ATurnActor_Test::BeginPlay()
{
	Super::BeginPlay();

	m_attackStrategy = NewObject<USmithTurnBaseAIAttackStrategy>(this);
	check(m_attackStrategy != nullptr);
	m_moveStrategy = NewObject<USmithTurnBaseAIMoveStrategy>(this);
	check(m_moveStrategy != nullptr);
	m_idleStrategy = NewObject<USmithTurnBaseAIIdleStrategy>(this);
	check(m_idleStrategy != nullptr);

  AnimComponent->SwitchAnimState(TEXT("Idle"));

	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		m_logSystem = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
	}
}

void ATurnActor_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->StopBehaviorProcessor();
		m_aiBehaviorProcessor->MarkAsGarbage();
	}
}

void ATurnActor_Test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_bIsPlayingDeadAnimation)
	{
		if (AnimComponent != nullptr && AnimComponent->IsCurrentAnimationFinish())
		{
			SetActorTickEnabled(false);
			Destroy();
		}
		AnimComponent->UpdateAnim(DeltaTime);

		return;
	}

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->TickBehaviorProcessor(DeltaTime);
	}
}

void ATurnActor_Test::OnAttack(AttackHandle&& handle)
{
	if (handle.AttackPower > 0)
	{
		if (handle.AttackFrom != EDirection::Invalid)
		{
			const EDirection faceTo = StaticCast<EDirection>((StaticCast<uint8>(handle.AttackFrom) + 4u) % StaticCast<uint8>(EDirection::DirectionCount)); 
			faceToDirection(faceTo);
		}
		EnemyParam.HP -= handle.AttackPower;

		if (m_logSystem != nullptr)
		{
			m_logSystem->SendAttackLog(handle.Attacker, this);
			m_logSystem->SendDamageLog(this,handle.AttackPower);
		}

		if (AnimComponent != nullptr)
		{
			AnimComponent->SwitchAnimState(TEXT("Damaged"));
		}
	}
	else
	{
		return;
	}

	if (EnemyParam.HP <= 0)
	{
		if (m_logSystem != nullptr)
		{
			m_logSystem->SendDefeatedLog(this);
		}

		if (m_eventMediator.IsValid())
		{
			IPickable* pickable = FSmithEnemyLootGenerator::GetLoot(this);
			if (pickable != nullptr)
			{
				USmithPickable* smithPickable = Cast<USmithPickable>(pickable);
				m_eventMediator->PublishPickUpEvent(this, smithPickable);
			}
		}

		if (OnDefeatEvent.IsBound())
		{
			OnDefeatEvent.Broadcast();
		}

		if (AnimComponent != nullptr)
		{
			AnimComponent->SwitchAnimState(TEXT("Dead"));
			m_bIsPlayingDeadAnimation = true;
		}
	}
}

uint8 ATurnActor_Test::GetOnMapSizeX() const
{
	return 1;
}

uint8 ATurnActor_Test::GetOnMapSizeY() const
{
	return 1;
}

UClass* ATurnActor_Test::GetMoveDirectorUClass() const
{
	return MoveDirectorSubclass.Get();
}

void ATurnActor_Test::SetMoveDirector(USmithMoveDirector* director)
{
	m_moveDirector = director;
}

uint8 ATurnActor_Test::GetChaseRadius() const
{
	return ChaseRadius;
}

EMapObjType ATurnActor_Test::GetType() const
{
	return MapObjectType;
}

void ATurnActor_Test::TurnOnAI()
{
	if (m_attackStrategy != nullptr)
	{
		m_attackStrategy->SetOwner(this);
		m_attackStrategy->Initialize(AtkComponent, m_commandMediator.Get(), EnemyParam.ATK);
		m_attackStrategy->SetAttackParam(EnemyParam.ATK, EnemyParam.CRT, m_level);
		m_attackStrategy->OnChangeDirectionDelegate.BindUObject(this, &ATurnActor_Test::faceToDirection);
	}

	for (auto& pair : AttackFormatTables)
	{
		if (!pair.Value.IsValid())
		{
			pair.Value.LoadSynchronous();
		}

		if (m_attackStrategy != nullptr)
		{
			m_attackStrategy->RegisterAttackFormat(pair.Key, pair.Value.Get());
		}
	}

	if (m_moveStrategy != nullptr)
	{
		m_moveStrategy->SetOwner(this);
		m_moveStrategy->Initialize(m_commandMediator.Get(), m_moveDirector, MoveComponent, 1);
		m_moveStrategy->OnMoveToEvent.BindUObject(this, &ATurnActor_Test::faceToDirection);
	}

	if (m_idleStrategy != nullptr)
	{
		m_idleStrategy->SetOwner(this);
		m_idleStrategy->Initialize(m_commandMediator.Get());
	}

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->RegisterAIStrategy(0, m_attackStrategy);
		m_aiBehaviorProcessor->RegisterAIStrategy(1, m_moveStrategy);
		m_aiBehaviorProcessor->RegisterAIStrategy(2, m_idleStrategy);
		m_aiBehaviorProcessor->RunBehaviorProcessor();
	}
}

void ATurnActor_Test::SetEventPublishMediator(IEventPublishMediator* eventMediator)
{
	m_eventMediator = eventMediator;
}

void ATurnActor_Test::SwitchAnimation(uint8 animationState)
{
	//MDebug::Log(TEXT("called animation"));

	if (AnimComponent == nullptr)
	{
		return;
	}

	using namespace UE::Smith;
	FName StateName;
	switch (animationState)
	{
	case SMITH_ANIM_IDLE:
		StateName = TEXT("Idle");
		break;
	case	SMITH_ANIM_WALK:
		StateName = TEXT("Walk");
		break;
	case SMITH_ANIM_ATTACK:
		StateName = TEXT("Attack");
		break;
	case SMITH_ANIM_DAMAGED:
		StateName = TEXT("Damaged");
		break;
	case SMITH_ANIM_DEAD:
		StateName = TEXT("Dead");
		break;
	default:
		break;
	}
	AnimComponent->SwitchAnimState(StateName);
}

void ATurnActor_Test::UpdateAnimation(float deltaTime)
{
	AnimComponent->UpdateAnim(deltaTime);
}

void ATurnActor_Test::SwitchAnimationDelay(uint8 animationState, float delay)
{
  using namespace UE::Smith;
	FName StateName;
	switch (animationState)
	{
	case SMITH_ANIM_IDLE:
		StateName = TEXT("Idle");
		break;
	case	SMITH_ANIM_WALK:
		StateName = TEXT("Walk");
		break;
	case SMITH_ANIM_ATTACK:
		StateName = TEXT("Attack");
		break;
	case SMITH_ANIM_DAMAGED:
		StateName = TEXT("Damaged");
		break;
	case SMITH_ANIM_DEAD:
		StateName = TEXT("Dead");
		break;
	default:
		break;
	}
  AnimComponent->SwitchAnimStateDelay(StateName, delay);
}

bool ATurnActor_Test::IsAnimationFinish() const
{
	return AnimComponent == nullptr ? true : AnimComponent->IsCurrentAnimationFinish();
}

FString ATurnActor_Test::GetName_Log() const
{
	return TEXT("小ゴーレム");
}

EBattleLogType ATurnActor_Test::GetType_Log() const
{
	return EBattleLogType::Enemy;
}

void ATurnActor_Test::InitializeParameter(int32 currentLevel)
{
	EnemyParam = FSmithEnemyParamInitializer::GetParams(*this, currentLevel);
	// TODO
	m_level = 1 + (currentLevel - 1) * 3;
}

void ATurnActor_Test::faceToDirection(EDirection newDirection)
{
	if (StaticCast<uint8>(newDirection) >= StaticCast<uint8>(EDirection::DirectionCount))
	{
		return;
	}

	const double newYaw = StaticCast<double>(newDirection) * 360.0 / StaticCast<double>(EDirection::DirectionCount);
	SetActorRotation(FRotator{0.0, newYaw, 0.0});
}

FBattleDefenseParamHandle ATurnActor_Test::GetDefenseParam() const
{
	FBattleDefenseParamHandle handle;
	handle.DefensePoint = EnemyParam.DEF;
	handle.Level = m_level;
	return handle;
}
