// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithBoss.h"
#include "AttackHandle.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithAIConditionAttackStrategy.h"
#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithTurnBaseAIIdleStrategy.h"
#include "SmithAttackComponent.h"
#include "FormatInfo_Import.h"
#include "SmithPickable.h"
#include "IEventPublishMediator.h"
#include "SmithBattleLogWorldSubsystem.h"
#include "SmithEnemyParamInitializer.h"
#include "MLibrary.h"

// TODO
#include "SmithDangerZoneDisplayer.h"

ASmithBoss::ASmithBoss()
  : m_attackStrategy(nullptr)
  , m_idleStrategy(nullptr)
  , m_atkComponent(nullptr)
  , m_wingsCnt(0)
  , m_breathCnt(0)
  , m_sweepCnt(0)
  , m_isRage(false)
{
  PrimaryActorTick.bCanEverTick = true;
  SetTurnPriority(ETurnPriority::Rival);

  m_atkComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("attack comp test"));
  check(m_atkComponent != nullptr);
}

void ASmithBoss::BeginPlay()
{
  Super::BeginPlay();

  m_attackStrategy = NewObject<USmithAIConditionAttackStrategy>(this);
  check(m_attackStrategy != nullptr);
  m_moveStrategy = NewObject<USmithTurnBaseAIMoveStrategy>(this);
  check(m_moveStrategy != nullptr);
  m_idleStrategy = NewObject<USmithTurnBaseAIIdleStrategy>(this);
  check(m_idleStrategy != nullptr);

  UWorld* world = GetWorld();
  if (::IsValid(world))
  {
    m_logSystem = world->GetSubsystem<USmithBattleLogWorldSubsystem>();
  }

  m_maxHp = EnemyParam.HP;
}

void ASmithBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  if (m_aiBehaviorProcessor != nullptr)
  {
    m_aiBehaviorProcessor->StopBehaviorProcessor();
    m_aiBehaviorProcessor->MarkAsGarbage();
  }
}

void ASmithBoss::Tick(float DeltaTime)
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

void ASmithBoss::OnAttack(AttackHandle&& handle)
{
  EnemyParam.HP -= handle.AttackPower;

  if (m_logSystem != nullptr)
  {
    // TODO
    m_logSystem->SendAttackLog(handle.Attacker, this);
    m_logSystem->SendDamageLog(this, handle.AttackPower);
  }
  if(EnemyParam.HP <= 0)
  {
    Destroy();
  }
}

uint8 ASmithBoss::GetOnMapSizeX() const
{
  return 3;
}

uint8 ASmithBoss::GetOnMapSizeY() const
{
  return 3;
}

EMapObjType ASmithBoss::GetType() const
{
  return MapObjectType;
}

void ASmithBoss::TurnOnAI()
{
  if (m_attackStrategy != nullptr)
  {
    m_attackStrategy->SetOwner(this);
    m_attackStrategy->Initialize(m_atkComponent, m_commandMediator.Get(), EnemyParam.ATK);
  }

	for (const auto& [AttackName, ConditionBindHandle] : ConditionAttackFormatTables)
	{
		if (!ConditionBindHandle.FormatMasterData.IsValid())
		{
			ConditionBindHandle.FormatMasterData.LoadSynchronous();
		}

    if (m_attackStrategy != nullptr)
    {
      TDelegate<bool()> condition;
      condition.BindUFunction(this, ConditionBindHandle.ConditionFuncName);
      m_attackStrategy->ConditionResgister(AttackName, ConditionBindHandle.FormatMasterData.Get(), condition, ConditionBindHandle.SkillParameter);
    }
	}

  if (m_idleStrategy != nullptr)
  {
    m_idleStrategy->SetOwner(this);
    m_idleStrategy->Initialize(m_commandMediator.Get());
  }

  if (m_aiBehaviorProcessor != nullptr)
  {  
    m_aiBehaviorProcessor->RegisterAIStrategy(0, m_attackStrategy);
    m_aiBehaviorProcessor->RegisterAIStrategy(1, m_idleStrategy);   
		m_aiBehaviorProcessor->RunBehaviorProcessor();
  }
}

void ASmithBoss::SetEventPublishMediator(IEventPublishMediator* eventMediator)
{
  m_eventMediator = eventMediator;
}

bool ASmithBoss::RageCondition()
{
    float currHp = StaticCast<float>(EnemyParam.HP) / StaticCast<float>(m_maxHp);
    float conditionHP = m_maxHp * 0.5f; 

    // 怒りフラグが立っておらず条件以下のHPを下回ったら怒り攻撃
    if((currHp < conditionHP) && (!m_isRage))
    {
      m_isRage = true;
      return true;
    }

    return false;
}

bool ASmithBoss::WingsCondition()
{
  m_wingsCnt++;
  if(m_wingsCnt >= 5)
  {
    m_wingsCnt = 0;
    return true;
  }

  return false;
}

bool ASmithBoss::BreathCondition()
{
  m_breathCnt++;
  if(m_breathCnt >= 3)
  {
    m_breathCnt = 0;
    return true;
  }

  return false;
}

bool ASmithBoss::SweepCondition()
{
  m_sweepCnt++;
  if(m_sweepCnt >= 3)
  {
    m_sweepCnt = 0;
    return true;
  }

  return false;
}

bool ASmithBoss::NormalCondition()
{
  return true;
}

FString ASmithBoss::GetName_Log() const
{
  return Name;
}

EBattleLogType ASmithBoss::GetType_Log() const
{
  return EBattleLogType::Enemy;
}

void ASmithBoss::InitializeParameter(int32 currentLevel)
{
	EnemyParam = FSmithEnemyParamInitializer::GetParams(this, currentLevel);
}