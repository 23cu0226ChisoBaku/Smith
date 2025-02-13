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
  , m_bIsDisposed(false)
{ }

void USmithNextLevelEvent::BeginDestroy()
{
  if (m_eventAppearance != nullptr)
  {
    m_eventAppearance->Destroy();
  }

  Super::BeginDestroy();
}

void USmithNextLevelEvent::InitializeEvent(const FVector& location, const FRotator& rotation)
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

      m_eventAppearance = world->SpawnActor<AActor>(subClass.Get(), location, rotation);
    }
  }
  else
  {
    m_eventAppearance->SetActorLocationAndRotation(location, rotation);
    m_eventAppearance->SetActorHiddenInGame(false);
  }

  m_bIsDisposed = false;
}

void USmithNextLevelEvent::TriggerEvent(ICanSetOnMap* mapObj)
{
  if (!IS_UINTERFACE_VALID(mapObj))
  {
    return;
  }

  if (mapObj->GetType() != EMapObjType::Player)
  {
    return;
  }

  IEventTriggerable* eventTriggerable = Cast<IEventTriggerable>(mapObj);
  if (eventTriggerable == nullptr)
  {
    return;
  }

  eventTriggerable->OnTriggerEvent(this);
}

void USmithNextLevelEvent::DiscardEvent()
{
  if (m_eventAppearance != nullptr)
  {
    m_eventAppearance->SetActorHiddenInGame(true);
  }
}

void USmithNextLevelEvent::RaiseEvent()
{
  MLibrary::UE::Audio::AudioKit::PlaySE("Next_Level");
  m_bIsDisposed = true;
  OnNextLevel.ExecuteIfBound();
}

bool USmithNextLevelEvent::IsDisposed() const
{ 
  return m_bIsDisposed;
}