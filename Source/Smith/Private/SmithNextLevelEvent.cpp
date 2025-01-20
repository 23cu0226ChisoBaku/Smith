// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithNextLevelEvent.h"
#include "ICanSetOnMap.h"
#include "MapObjType.h"
#include "IEventTriggerable.h"
#include "MLibrary.h"

USmithNextLevelEvent::USmithNextLevelEvent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , OnNextLevel{}
  , m_eventAppearance(nullptr)
{ }

void USmithNextLevelEvent::BeginDestroy()
{
  if (m_eventAppearance != nullptr)
  {
    m_eventAppearance->Destroy();
  }

  Super::BeginDestroy();
}

void USmithNextLevelEvent::InitializeEvent(const FVector& location)
{  
  if (m_eventAppearance == nullptr)
  {
    UWorld* world = GetWorld();
    if (world != nullptr)
    {
      TSubclassOf<class AActor> subClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("/Game/BP/BP_TestStairs.BP_TestStairs_C"))).LoadSynchronous();

      if (subClass == nullptr)
      {
        MDebug::LogError("Invalid next level Path");
        return;
      }

      m_eventAppearance = world->SpawnActor<AActor>(subClass.Get(), location, FRotator::ZeroRotator);
    }
  }
}

bool USmithNextLevelEvent::TriggerEvent(ICanSetOnMap* mapObj)
{
  if (!IS_UINTERFACE_VALID(mapObj))
  {
    return false;
  }

  if (mapObj->GetType() != EMapObjType::Player)
  {
    return false;
  }

  IEventTriggerable* eventTriggerable = Cast<IEventTriggerable>(mapObj);
  if (eventTriggerable != nullptr)
  {
    eventTriggerable->OnTriggerEvent(this);
  }

  MDebug::LogWarning("Trigger Next Level Event");
  OnNextLevel.ExecuteIfBound();
  return true;
}

void USmithNextLevelEvent::DiscardEvent()
{
  if (m_eventAppearance != nullptr)
  {
    m_eventAppearance->SetActorHiddenInGame(true);
  }

  MarkAsGarbage();
}