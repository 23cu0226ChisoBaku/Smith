// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIMoveStrategy.h"
#include "SmithMoveDirector.h"
#include "ICommandMediator.h"
#include "IMoveable.h"
#include "Direction.h"
#include "MLibrary.h"

USmithTurnBaseAIMoveStrategy::USmithTurnBaseAIMoveStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mediator(nullptr)
  , m_moveDirector(nullptr)
  , m_move(nullptr)
  , m_moveSpeed(0)
{ }

void USmithTurnBaseAIMoveStrategy::Initialize(ICommandMediator* mediator, USmithMoveDirector* moveDirector, IMoveable* move, uint8 moveSpeed)
{
  m_mediator = mediator;
  m_moveDirector = moveDirector;
  m_move = move;
  m_moveSpeed = moveSpeed;
}

void USmithTurnBaseAIMoveStrategy::BeginDestroy()
{
  m_mediator.Reset();
  m_moveDirector.Reset();
  m_move.Reset();
  m_moveSpeed = 0;
  
  Super::BeginDestroy();
}

bool USmithTurnBaseAIMoveStrategy::executeImpl()
{
  if (!m_mediator.IsValid() || !m_moveDirector.IsValid() || !m_move.IsValid())
  {
    MDebug::LogError("not initialize -- move strategy");
    return false;
  }

  EDirection moveDir = m_moveDirector->GetNextDirection();
  
  bool success = m_mediator->SendMoveCommand(GetOwner(), m_move.Get(), moveDir, m_moveSpeed);
  if (success)
  {
    if (OnMoveToEvent.IsBound())
    {
      OnMoveToEvent.Execute(moveDir);
    }
  }
  return success;
}
