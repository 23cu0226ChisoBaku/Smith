// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategy.h"
#include "MLibrary.h"

USmithAIBehaviorProcessor::USmithAIBehaviorProcessor(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , TickConditionDelegate{}
  , m_strategyContainer{}
  , m_bIsProcessorRunning(false)
{ }

void USmithAIBehaviorProcessor::BeginDestroy()
{
  Super::BeginDestroy();

  m_bIsProcessorRunning = false;
  for (auto& strategy : m_strategyContainer)
  {
    if (strategy.Strategy != nullptr)
    {
      strategy.Strategy->ConditionalBeginDestroy();
    }
  }
  m_strategyContainer.Empty();
}

void USmithAIBehaviorProcessor::RegisterAIStrategy(int32 priority, USmithAIStrategy* strategy)
{
  if (m_bIsProcessorRunning)
  {
    MDebug::LogError("Can not register during processor running");
    return;
  }

  if (strategy == nullptr)
  {
    return;
  }

  m_strategyContainer.Emplace(FSmithAIStrategyContainer{priority, strategy});

  m_strategyContainer.Sort();
}

void USmithAIBehaviorProcessor::EmptyStrategy()
{
  if (m_bIsProcessorRunning)
  {
    MDebug::LogError("Can not EMPTY during processor running");
    return;
  }

  m_strategyContainer.Reset();
}

void USmithAIBehaviorProcessor::RunBehaviorProcessor()
{
  m_bIsProcessorRunning = true;
}

void USmithAIBehaviorProcessor::TickBehaviorProcessor(float deltaTime)
{
  if (!m_bIsProcessorRunning)
  {
    return;
  }

  if (TickConditionDelegate.IsBound())
  {
    if (!TickConditionDelegate.Execute())
    {
      return;
    }
  }

  for (auto& strategy : m_strategyContainer)
  {
    if (strategy.Strategy == nullptr)
    {
      continue;
    }

    bool success = strategy.Strategy->Execute();
    if (success)
    {
      break;
    }
  }
}

void USmithAIBehaviorProcessor::StopBehaviorProcessor()
{
  m_bIsProcessorRunning = false;
}

bool operator<(const FSmithAIStrategyContainer& lhs, const FSmithAIStrategyContainer& rhs)
{
  return lhs.Priority < rhs.Priority;
}