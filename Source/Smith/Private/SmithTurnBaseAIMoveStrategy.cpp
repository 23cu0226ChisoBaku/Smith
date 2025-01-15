// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIMoveStrategy.h"
#include "ICommandMediator.h"
#include "IMoveable.h"
#include "MoveDirection.h"
#include "MLibrary.h"

USmithTurnBaseAIMoveStrategy::USmithTurnBaseAIMoveStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mediator(nullptr)
  , m_move(nullptr)
  , m_moveSpeed(0)
{ }

void USmithTurnBaseAIMoveStrategy::Initialize(ICommandMediator* mediator, IMoveable* move, uint8 moveSpeed)
{
  m_mediator = mediator;
  m_move = move;
  m_moveSpeed = moveSpeed;
}

void USmithTurnBaseAIMoveStrategy::BeginDestroy()
{
  Super::BeginDestroy();
  m_move = nullptr;
  m_moveSpeed = 0;
}

bool USmithTurnBaseAIMoveStrategy::executeImpl()
{
  using namespace UE::Smith::Battle;
  if (!m_mediator.IsValid() || !m_move.IsValid())
  {
    return false;
  }

  return m_mediator->SendMoveCommand(GetOwner(), m_move.Get(), EMoveDirection::North, m_moveSpeed);
}
