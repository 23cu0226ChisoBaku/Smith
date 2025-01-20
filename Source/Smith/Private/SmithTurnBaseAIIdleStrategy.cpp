// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIIdleStrategy.h"
#include "ICommandMediator.h"
#include "MLibrary.h"

USmithTurnBaseAIIdleStrategy::USmithTurnBaseAIIdleStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mediator(nullptr)
{ }

void USmithTurnBaseAIIdleStrategy::BeginDestroy()
{
  m_mediator.Reset();

  Super::BeginDestroy();
}

void USmithTurnBaseAIIdleStrategy::Initialize(ICommandMediator* mediator)
{
  m_mediator = mediator;
}

bool USmithTurnBaseAIIdleStrategy::executeImpl()
{
  if (!m_mediator.IsValid())
  {
    MDebug::LogError("not initialize -- idle strategy");
    return false;
  }

  return m_mediator->SendIdleCommand(GetOwner());
}