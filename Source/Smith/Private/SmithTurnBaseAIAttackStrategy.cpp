// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIAttackStrategy.h"
#include "ICommandMediator.h"
#include "ICanMakeAttack.h"
#include "Direction.h"
#include "AttackHandle.h"
#include "MLibrary.h"

USmithTurnBaseAIAttackStrategy::USmithTurnBaseAIAttackStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , OnChangeDirectionDelegate{}
  , m_mediator(nullptr)
  , m_attacker(nullptr)
{ }

void USmithTurnBaseAIAttackStrategy::Initialize(ICanMakeAttack* attacker, ICommandMediator* mediator)
{
  m_mediator = mediator;
  m_attacker = attacker;
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

  for (auto& format : m_attackFormatTables)
  {
    if (!format.Value.IsValid())
    {
      continue;
    }

    for (uint8 i = 0u; i < 4u; ++i)
    {
      EDirection atkDir = StaticCast<EDirection>(i * 2u);
      bool success = m_mediator->SendAttackCommand(GetOwner(), m_attacker.Get(), atkDir, *format.Value, AttackHandle{GetName(), 1}, false);
      if (success)
      {
        return true;
      }
    }
  }

  MDebug::LogError("Attack failed");
  return false;
}

