// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithBoss.h"

#include "AttackHandle.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithAIConditionAttackStrategy.h"
#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithTurnBaseAIIdleStrategy.h"
#include "SmithAnimationComponent.h"
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
  , AnimComponent(nullptr)
  , m_wingsCnt(0)
  , m_breathCnt(0)
  , m_sweepCnt(0)
  , m_isRage(false)
{
  PrimaryActorTick.bCanEverTick = true;
  SetTurnPriority(ETurnPriority::Rival);

  AnimComponent = CreateDefaultSubobject<USmithAnimationComponent>(TEXT("anim comp"));
  check(AnimComponent != nullptr)
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

  AnimComponent->SwitchAnimState(TEXT("Idle"));

  m_maxHp = EnemyParam.HP;
}

void ASmithBoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  if (m_aiBehaviorProcessor != nullptr)
  {
    m_aiBehaviorProcessor->StopBehaviorProcessor();
    m_aiBehaviorProcessor->ConditionalBeginDestroy();
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

void ASmithBoss::OnAttack(const FBattleResult& Result)
{
  EnemyParam.HP -= Result.Damage;
}

bool ASmithBoss::IsDefeated() const
{
  return EnemyParam.HP <= 0;
}

void ASmithBoss::OnDefeated()
{
  if (OnDefeatEvent.IsBound())
  {
    OnDefeatEvent.Broadcast();
  }
  Destroy();
}

void ASmithBoss::TurnOnAI()
{
  if (m_attackStrategy != nullptr)
  {
    m_attackStrategy->SetOwner(this);
    m_attackStrategy->Initialize(m_commandMediator.Get(), EnemyParam.ATK);
    m_attackStrategy->SetAttackParam(EnemyParam.ATK, EnemyParam.CRT, m_level);
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
    m_idleStrategy->Initialize(m_commandMediator.Get(), 0.2f);
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

void ASmithBoss::SwitchAnimation(uint8 animationState)
{
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
	case SMITH_ANIM_ATTACK:
		StateName = TEXT("Attack");
		break;
	case SMITH_ANIM_DAMAGED:
		StateName = TEXT("Damaged");
		break;
	case SMITH_ANIM_DEAD:
		StateName = TEXT("Dead");
		break;
  case SMITH_ANIM_SKILL_ONE:
    StateName = TEXT("WingAttack_Active");
    break;
  case SMITH_ANIM_SKILL_TWO:
    StateName = TEXT("Breath_Active");
    break;
  case SMITH_ANIM_SKILL_THREE:
    StateName = TEXT("Press_Active");
	default:
		break;
	}
	AnimComponent->SwitchAnimState(StateName);
}

void ASmithBoss::UpdateAnimation(float deltaTime)
{
	AnimComponent->UpdateAnim(deltaTime);
}

bool ASmithBoss::IsAnimationFinish() const
{
	return AnimComponent == nullptr ? true : AnimComponent->IsCurrentAnimationFinish();
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
  if (m_wingsCnt == 0 && AnimComponent != nullptr)
  {
    AnimComponent->SwitchAnimState(TEXT("WingAttack"));
  }
  m_wingsCnt++;
  if(m_wingsCnt >= 2)
  {
    MLibrary::UE::Audio::AudioKit::PlaySE3D(TEXT("Dragon_Wing"), 0.7f, GetActorLocation());
    m_wingsCnt = 0;
    return true;
  }

  return false;
}

bool ASmithBoss::BreathCondition()
{
  if (m_breathCnt == 0 && AnimComponent != nullptr)
  {
    AnimComponent->SwitchAnimState(TEXT("Breath"));
  }
  m_breathCnt++;
  if(m_breathCnt >= 3)
  {
    MLibrary::UE::Audio::AudioKit::PlaySE3D(TEXT("Dragon_Breath"), 0.7f, GetActorLocation());
    m_breathCnt = 0;
    return true;
  }

  return false;
}

bool ASmithBoss::PressCondition()
{
  if (m_sweepCnt == 0 && AnimComponent != nullptr)
  {
    AnimComponent->SwitchAnimState(TEXT("Press"));
  }
  m_sweepCnt++;
  if(m_sweepCnt >= 3)
  {
    MLibrary::UE::Audio::AudioKit::PlaySE3D(TEXT("Dragon_Press"), 0.7f, GetActorLocation());
    m_sweepCnt = 0;
    return true;
  }

  return false;
}

bool ASmithBoss::NormalCondition()
{
  return true;
}

void ASmithBoss::InitializeParameter(int32 currentLevel)
{
	EnemyParam = USmithEnemyParamInitializer::GetParams(this, currentLevel);
  m_level = 1 + (currentLevel - 1) * 4;
}