// Fill out your copyright notice in the Description page of Project Settings.


#include "HerbGolem.h"
#include "AttackHandle.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithTurnBaseAIAttackStrategy.h"
#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithTurnBaseAIIdleStrategy.h"

#include "SmithAnimationComponent.h"

#include "FormatInfo_Import.h"
#include "SmithMoveDirector.h"
#include "SmithPickable.h"
#include "IEventPublishMediator.h"
#include "SmithBattleLogWorldSubsystem.h"
#include "MLibrary.h"

#include "SmithEnemyParamInitializer.h"

AHerbGolem::AHerbGolem()
	: m_moveStrategy(nullptr)
	, m_idleStrategy(nullptr)
	, AnimComponent(nullptr)
	, m_healCnt(1)
{
	PrimaryActorTick.bCanEverTick = true;
	SetTurnPriority(ETurnPriority::Rival);

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

void AHerbGolem::OnAttack(const AttackHandle& Handle)
{
	if (Handle.AttackPower > 0)
	{
		EnemyParam.HP -= Handle.AttackPower;

	}
	else
	{
		return;
	}

	if (EnemyParam.HP <= 0)
	{

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

void AHerbGolem::TurnOnAI()
{
	if (m_attackStrategy != nullptr)
	{
		m_attackStrategy->SetOwner(this);
		m_attackStrategy->Initialize(m_commandMediator.Get(), EnemyParam.ATK);
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
		m_moveStrategy->Initialize(m_commandMediator.Get(), m_moveDirector, 1);
	}

	if (m_idleStrategy != nullptr)
	{
		m_idleStrategy->SetOwner(this);
		m_idleStrategy->Initialize(m_commandMediator.Get());
	}

	if (m_aiBehaviorProcessor != nullptr)
	{
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

bool AHerbGolem::IsAnimationFinish() const
{
	return AnimComponent == nullptr ? true : AnimComponent->IsCurrentAnimationFinish();
}

void AHerbGolem::InitializeParameter(int32 currentLevel)
{
	EnemyParam = USmithEnemyParamInitializer::GetParams(this, currentLevel);
  m_maxHp = EnemyParam.HP;
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