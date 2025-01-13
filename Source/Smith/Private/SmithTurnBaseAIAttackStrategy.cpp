// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIAttackStrategy.h"
#include "ICommandMediator.h"
#include "ICanMakeAttack.h"
#include "MoveDirection.h"
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
  Super::BeginDestroy();
  m_mediator = nullptr;
  m_attacker = nullptr;
  OnChangeDirectionDelegate.Unbind();
}

bool USmithTurnBaseAIAttackStrategy::executeImpl()
{
  if (m_attackFormatTables.Num() == 0)
  {
    return false;
  }

  if (!m_mediator.IsValid() || !m_attacker.IsValid())
  {
    return false;
  }

  for (auto& format : m_attackFormatTables)
  {
    m_mediator->SendAttackCommand(GetOwner(), m_attacker.Get(), UE::Smith::Battle::EMoveDirection::North, *format.Value, AttackHandle{GetName(), 1}); 
  }

  return true;
}


