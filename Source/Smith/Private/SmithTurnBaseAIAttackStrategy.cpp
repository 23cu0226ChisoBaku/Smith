// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIAttackStrategy.h"
#include "ICommandMediator.h"
#include "ICanMakeAttack.h"
#include "Direction.h"
#include "AttackHandle.h"
#include "ISmithBattleLogger.h"
#include "SmithModelHelperFunctionLibrary.h"
#include "MLibrary.h"

USmithTurnBaseAIAttackStrategy::USmithTurnBaseAIAttackStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , OnChangeDirectionDelegate{}
  , m_mediator(nullptr)
  , m_attacker(nullptr)
  , m_atk(0)
  , m_crt(0)
  , m_level(0)
{ }

void USmithTurnBaseAIAttackStrategy::Initialize(ICanMakeAttack* attacker, ICommandMediator* mediator, int32 attackPower)
{
  m_mediator = mediator;
  m_attacker = attacker;
  m_atk = attackPower;
}

void USmithTurnBaseAIAttackStrategy::SetAttackParam(int32 attackPower, int32 critical, int32 level)
{
  m_atk = attackPower;
  m_crt = critical;
  m_level = level;
}

void USmithTurnBaseAIAttackStrategy::BeginDestroy()
{
  m_mediator = nullptr;
  m_attacker = nullptr;
  OnChangeDirectionDelegate.Unbind();
  
  Super::BeginDestroy();
}

bool USmithTurnBaseAIAttackStrategy::executeImpl()
{
  if (m_attackFormatTables.Num() == 0)
  {
    MDebug::LogError("No format");
    return false;
  }

  if (!m_mediator.IsValid() || !m_attacker.IsValid())
  {
    MDebug::LogError("not initialize -- attack strategy");
    return false;
  }

  ISmithBattleLogger* attackerLogger = Cast<ISmithBattleLogger>(GetOwner());
  FAttackHandle handle;
  handle.Attacker = attackerLogger;
  handle.AttackPower = m_atk;
  handle.CriticalPower = m_crt;
  handle.Level = m_level;
  handle.MotionValue = 1.0;
  for (auto& format : m_attackFormatTables)
  {
    if (!format.Value.IsValid())
    {
      continue;
    }

    
    for (uint8 i = 0u; i < 4u; ++i)
    {
      EDirection atkDir = StaticCast<EDirection>(i * 2u);
      bool success = m_mediator->SendAttackCommand(GetOwner(), m_attacker.Get(), atkDir, *format.Value, handle, false);
      if (success)
      {
        if (OnChangeDirectionDelegate.IsBound())
        {
          
          m_mediator->GetPlayerDirection(atkDir, GetOwner());
          OnChangeDirectionDelegate.Execute(atkDir);
        }
        return true;
      }
    }
  }

  return false;
}


