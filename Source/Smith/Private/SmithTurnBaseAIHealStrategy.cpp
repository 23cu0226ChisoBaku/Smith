// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTurnBaseAIHealStrategy.h"
#include "ICommandMediator.h"
#include "MLibrary.h"

USmithTurnBaseAIHealStrategy::USmithTurnBaseAIHealStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mediator(nullptr)
  , m_healable(nullptr)
{ }

void USmithTurnBaseAIHealStrategy::BeginDestroy()
{
  m_mediator.Reset();

  Super::BeginDestroy();
}

void USmithTurnBaseAIHealStrategy::Initialize(ICommandMediator* mediator,IHealable* healable)
{
  m_mediator = mediator;
  m_healable = healable;
}

void USmithTurnBaseAIHealStrategy::RegisterCondition(const TDelegate<bool(void)>& condition)
{
  if(condition.IsBound())
  {
    m_condition = condition;
  }
  else
  {
    MDebug::LogError("Condition is not bound");
  }
}

bool USmithTurnBaseAIHealStrategy::executeImpl()
{
  if (!m_mediator.IsValid())
  {
    MDebug::LogError("not initialize -- idle strategy");
    return false;
  }

  if(m_healable == nullptr)
  {
    MDebug::LogError("Healable is nullptr");
    return false;
  }

  if(m_condition.Execute() == false)
  {
    return false;
  }
  
  return m_mediator->SendHealCommand(GetOwner(),m_healable);
}
