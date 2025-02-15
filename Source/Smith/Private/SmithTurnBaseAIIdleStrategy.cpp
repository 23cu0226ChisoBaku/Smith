// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIIdleStrategy.h"
#include "ICommandMediator.h"
#include "MLibrary.h"

USmithTurnBaseAIIdleStrategy::USmithTurnBaseAIIdleStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mediator(nullptr)
  , m_idleTime(0.0f)
{ }

void USmithTurnBaseAIIdleStrategy::BeginDestroy()
{
  m_mediator.Reset();

  Super::BeginDestroy();
}

void USmithTurnBaseAIIdleStrategy::Initialize(ICommandMediator* mediator, float idleTime)
{
  m_mediator = mediator;
  m_idleTime = idleTime;
}

bool USmithTurnBaseAIIdleStrategy::executeImpl()
{
  if (!m_mediator.IsValid())
  {
    MDebug::LogError("not initialize -- idle strategy");
    return false;
  }

  return m_mediator->SendIdleCommand(GetOwner(), m_idleTime);
}