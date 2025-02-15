// Fill out your copyright notice in the Description page of Project Settings.


#include "HerbGolem.h"
#include "AttackHandle.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithTurnBaseAIAttackStrategy.h"
#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithTurnBaseAIIdleStrategy.h"
#include "SmithTurnBaseAIHealStrategy.h"
#include "SmithAttackComponent.h"
#include "SmithMoveComponent.h"

#include "SmithAnimationComponent.h"

#include "FormatInfo_Import.h"
#include "SmithMoveDirector.h"
#include "SmithPickable.h"
#include "IEventPublishMediator.h"
#include "SmithBattleLogWorldSubsystem.h"
#include "MLibrary.h"

#include "SmithEnemyParamInitializer.h"

AHerbGolem::AHerbGolem()
	: m_attackStrategy(nullptr)
	, m_moveStrategy(nullptr)
	, m_idleStrategy(nullptr)
  , m_healStrategy(nullptr)
	, m_atkComponent(nullptr)
	, MoveComponent(nullptr)
	, AnimComponent(nullptr)
	, m_healCnt(1)
{
	PrimaryActorTick.bCanEverTick = true;
	SetTurnPriority(ETurnPriority::Rival);

	m_atkComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("attack comp test"));
	check(m_atkComponent != nullptr);

	MoveComponent = CreateDefaultSubobject<USmithMoveComponent>(TEXT("move comp test"));
	check(MoveComponent != nullptr);

	AnimComponent = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("anim comp"));
	check(AnimComponent != nullptr)

}

void AHerbGolem::BeginPlay()
{
	Super::BeginPlay();

	m_attackStrategy = NewObject<USmithTurnBaseAIAttackStrategy>(this);
	check(m_attackStrategy != nullptr);
	m_moveStrategy = NewObject<USmithTurnBaseAIMoveStrategy>(this);
	check(m_moveStrategy != nullptr);
	m_idleStrategy = NewObject<USmithTurnBaseAIIdleStrategy>(this);
	check(m_idleStrategy != nullptr);
  m_healStrategy = NewObject<USmithTurnBaseAIHealStrategy>(this);
  check(m_healStrategy != nullptr);

  AnimComponent->SwitchAnimState(TEXT("Idle"));

	UWorld* world = GetWorld();
	if (world != nullptr)
	{
		m_logSystem = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
	}

  // 仮のステータス
  EnemyParam.HP = 20;
  EnemyParam.ATK = 1;
}

void AHerbGolem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->StopBehaviorProcessor();
		m_aiBehaviorProcessor->MarkAsGarbage();
	}
}

void AHerbGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->TickBehaviorProcessor(DeltaTime);
	}
}

void AHerbGolem::OnAttack(AttackHandle&& handle)
{
	if (handle.AttackPower > 0)
	{
		EnemyParam.HP -= handle.AttackPower;

		if (m_logSystem != nullptr)
		{
			m_logSystem->SendAttackLog(handle.Attacker, this);
			m_logSystem->SendDamageLog(this,handle.AttackPower);
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
			if (DropUpgradeTable.Num() > 0)
			{
				int32 idx = FMath::RandRange(0, DropUpgradeTable.Num() - 1);
				m_eventMediator->PublishPickUpEvent(this, DropUpgradeTable[idx]);
			}
		}

		if (OnDefeatEvent.IsBound())
		{
			OnDefeatEvent.Broadcast();
		}

		Destroy();
		DropUpgradeTable.Reset();
	}
}

uint8 AHerbGolem::GetOnMapSizeX() const
{
	return 1;
}

uint8 AHerbGolem::GetOnMapSizeY() const
{
	return 1;
}

UClass* AHerbGolem::GetMoveDirectorUClass() const
{
	return MoveDirectorSubclass.Get();
}

void AHerbGolem::SetMoveDirector(USmithMoveDirector* director)
{
	m_moveDirector = director;
}

uint8 AHerbGolem::GetChaseRadius() const
{
	return ChaseRadius;
}

EMapObjType AHerbGolem::GetType() const
{
	return MapObjectType;
}

void AHerbGolem::TurnOnAI()
{
	if (m_attackStrategy != nullptr)
	{
		m_attackStrategy->SetOwner(this);
		m_attackStrategy->Initialize(m_atkComponent, m_commandMediator.Get(), EnemyParam.ATK);
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
	}

	if (m_idleStrategy != nullptr)
	{
		m_idleStrategy->SetOwner(this);
		m_idleStrategy->Initialize(m_commandMediator.Get());
	}

  if (m_healStrategy != nullptr)
  {
    m_healStrategy->SetOwner(this);
    m_healStrategy->Initialize(m_commandMediator.Get(),this);

    TDelegate<bool()> condition;
    condition.BindUObject(this,&AHerbGolem::HealCondition);
    m_healStrategy->RegisterCondition(condition);
  }

	if (m_aiBehaviorProcessor != nullptr)
	{
		m_aiBehaviorProcessor->RegisterAIStrategy(0, m_healStrategy);
		m_aiBehaviorProcessor->RegisterAIStrategy(1, m_attackStrategy);
		m_aiBehaviorProcessor->RegisterAIStrategy(2, m_moveStrategy);
    m_aiBehaviorProcessor->RegisterAIStrategy(3,m_idleStrategy);
		m_aiBehaviorProcessor->RunBehaviorProcessor();
	}
}

void AHerbGolem::SetEventPublishMediator(IEventPublishMediator* eventMediator)
{
	m_eventMediator = eventMediator;
}

void AHerbGolem::SwitchAnimation(uint8 animationState)
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

void AHerbGolem::UpdateAnimation(float deltaTime)
{
	AnimComponent->UpdateAnim(deltaTime);
}

void AHerbGolem::SwitchAnimationDelay(uint8 animationState, float delay)
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

bool AHerbGolem::IsAnimationFinish() const
{
	return AnimComponent == nullptr ? true : AnimComponent->IsCurrentAnimationFinish();
}

FString AHerbGolem::GetName_Log() const
{
	return TEXT("薬草ゴーレム");
}

EBattleLogType AHerbGolem::GetType_Log() const
{
	return EBattleLogType::Enemy;
}

void AHerbGolem::InitializeParameter(int32 currentLevel)
{
	EnemyParam = FSmithEnemyParamInitializer::GetParams(this, currentLevel);
  m_maxHp = EnemyParam.HP;
}

void AHerbGolem::Heal()
{
  float add = StaticCast<float>(m_maxHp) * 0.3f;
  EnemyParam.HP += add;
  if(EnemyParam.HP > m_maxHp)
  {
    EnemyParam.HP = m_maxHp;
  }
}

bool AHerbGolem::HealCondition()
{
  float hp = StaticCast<float>(EnemyParam.HP) / StaticCast<float>(m_maxHp);
  if((0.3f > hp) && (m_healCnt > 0))
  {
    --m_healCnt;
    return true;
  }

  return false;
}