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
      strategy.Strategy->MarkAsGarbage();
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

void USmithAIBehaviorProcessor::StopBehaviorProcessor()
{
  m_bIsProcessorRunning = false;
}

void USmithAIBehaviorProcessor::Tick(float DeltaTime)
{
  if (!m_bIsProcessorRunning)
  {
    return;
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
    else
    {
      MDebug::LogError("Move next strategy");
    }
    
  }
}
bool USmithAIBehaviorProcessor::IsTickable() const
{
  if (TickConditionDelegate.IsBound())
  {
    return TickConditionDelegate.Execute();
  }
  else
  {
    return false;
  }
} 

TStatId USmithAIBehaviorProcessor::GetStatId() const
{
  RETURN_QUICK_DECLARE_CYCLE_STAT(USmithAIBehaviorProcessor, STATGROUP_Tickables);
}

bool USmithAIBehaviorProcessor::IsTickableWhenPaused() const
{
  return false;
}

bool USmithAIBehaviorProcessor::IsTickableInEditor() const
{
  return true;
}

UWorld* USmithAIBehaviorProcessor::GetTickableGameObjectWorld() const
{
  return GetWorld();
}

bool operator<(const FSmithAIStrategyContainer& lhs, const FSmithAIStrategyContainer& rhs)
{
  return lhs.Priority < rhs.Priority;
}