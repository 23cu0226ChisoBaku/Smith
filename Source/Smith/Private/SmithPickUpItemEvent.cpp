// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPickUpItemEvent.h"
#include "IPickable.h"
#include "ICanPick.h"
#include "ICanSetOnMap.h"
#include "ISmithBattleLogger.h"
#include "IEventTriggerable.h"
#include "MLibrary.h"
USmithPickUpItemEvent::USmithPickUpItemEvent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_pickableAppearence(nullptr)
  , m_pickableObject(nullptr)
  , m_pickable(nullptr)
  , m_isPicked(true)
{ }

void USmithPickUpItemEvent::BeginDestroy()
{
  Super::BeginDestroy();
}

void USmithPickUpItemEvent::InitializeEvent(const FVector& location)
{
  if (m_pickableAppearence != nullptr)
  {
    m_pickableAppearence->SetActorLocation(location);
    m_pickableAppearence->SetActorHiddenInGame(false);
  }
}

void USmithPickUpItemEvent::TriggerEvent(ICanSetOnMap* mapObj)
{ 
  if (!m_pickable.IsValid())
  {
    DiscardEvent();
    return;
  }

  if (m_pickableObject == nullptr || !m_pickableObject->IsValidLowLevel())
  {
    DiscardEvent();
    return;
  }

  if (!IS_UINTERFACE_VALID(mapObj))
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

void USmithPickUpItemEvent::DiscardEvent()
{
  if (m_pickableAppearence != nullptr)
  {
    m_pickableAppearence->Destroy();
  }

  m_pickableObject = nullptr;
  m_pickable.Reset();

  MarkAsGarbage();
}

void USmithPickUpItemEvent::RaiseEvent()
{
  
  m_isPicked = true;
  if (m_pickableAppearence != nullptr)
  {
    m_pickableAppearence->SetActorHiddenInGame(true);
  }
}

bool USmithPickUpItemEvent::IsDisposed() const
{
  return m_isPicked;
}

void USmithPickUpItemEvent::AssignPickable(IPickable* pickable, AActor* appearance)
{
  if (m_pickable.IsValid())
  {
    return;
  }

  if (!IS_UINTERFACE_VALID(pickable))
  {
    return;
  }

  m_pickable = pickable;
  m_pickableObject = pickable->_getUObject();
  m_pickableObject->Rename(nullptr, this);
  m_pickableAppearence = appearance;

  if (m_pickableAppearence != nullptr)
  {
    m_pickableAppearence->SetActorHiddenInGame(true);
  }

  m_isPicked = false;
  
}

IPickable* USmithPickUpItemEvent::GetPickable() const
{
  return m_pickable.Get();
}

FString USmithPickUpItemEvent::GetPickUpItemType() const
{
  return m_pickable.IsValid() ? m_pickable->GetPickType() : TEXT("INVALID");
}

ISmithBattleLogger* USmithPickUpItemEvent::GetEventEntityLogger() const
{
  return Cast<ISmithBattleLogger>(m_pickable.Get());
}

FString USmithPickUpItemEvent::GetEventName() const
{
  return m_pickable.IsValid() ? m_pickable->GetPickType() : TEXT("とあるアイテム");
}

FString USmithPickUpItemEvent::GetSucceedMessage() const
{
  return TEXT("を手に入れた");
}

FString USmithPickUpItemEvent::GetFailedMessage() const
{
  return TEXT("の上に乗った");
}
