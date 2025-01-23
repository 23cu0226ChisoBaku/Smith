// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPickUpItemEvent.h"
#include "IPickable.h"
#include "ICanPick.h"
#include "ICanSetOnMap.h"
#include "MLibrary.h"
USmithPickUpItemEvent::USmithPickUpItemEvent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_pickableAppearence(nullptr)
  , m_pickableObject(nullptr)
  , m_pickable(nullptr)
{ }

void USmithPickUpItemEvent::BeginDestroy()
{
  Super::BeginDestroy();
  DiscardEvent();
}

void USmithPickUpItemEvent::InitializeEvent(const FVector& location)
{
  if (m_pickableAppearence != nullptr)
  {
    m_pickableAppearence->SetActorLocation(location);
    m_pickableAppearence->SetActorHiddenInGame(false);
  }
}

bool USmithPickUpItemEvent::TriggerEvent(ICanSetOnMap* mapObj)
{ 
  if (!m_pickable.IsValid())
  {
    DiscardEvent();
    return false;
  }

  if (m_pickableObject == nullptr || !m_pickableObject->IsValidLowLevel())
  {
    DiscardEvent();
    return false;
  }

  if (!IS_UINTERFACE_VALID(mapObj))
  {
    return false;
  }

  ICanPick* picker = Cast<ICanPick>(mapObj);
  if (picker == nullptr)
  {
    return false;
  }

  m_pickable->OnPick(picker);
  return true;
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
  m_pickableAppearence = appearance;

  if (m_pickableAppearence != nullptr)
  {
    m_pickableAppearence->SetActorHiddenInGame(true);
  }
  
}
