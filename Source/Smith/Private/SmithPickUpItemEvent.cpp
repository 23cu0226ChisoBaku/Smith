// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPickUpItemEvent.h"
#include "IPickable.h"
#include "ICanPick.h"
#include "ICanSetOnMap.h"
#include "ISmithBattleLogger.h"
#include "IEventTriggerable.h"
#include "MLibrary.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

USmithPickUpItemEvent::USmithPickUpItemEvent(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_pickableObject(nullptr)
  , m_pickable(nullptr)
  , m_isPicked(true)
{ }

void USmithPickUpItemEvent::BeginDestroy()
{
  Super::BeginDestroy();

  if (m_itemEventNiagaraComp != nullptr)
  {
    m_itemEventNiagaraComp->DeactivateImmediate();
  }
}

void USmithPickUpItemEvent::InitializeEvent(const FVector& location, const FRotator& rotation)
{
  m_itemEventNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
                                                                          m_itemEventNiagaraSystem,
                                                                          location,
                                                                          rotation);

  if (m_itemEventNiagaraComp != nullptr)
  {
    m_itemEventNiagaraComp->Activate();
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
  if (::IsValid(m_pickableObject))
  {
    m_pickableObject->ConditionalBeginDestroy();
  }
  m_pickableObject = nullptr;
  m_pickable.Reset();

  ConditionalBeginDestroy();
}

void USmithPickUpItemEvent::RaiseEvent()
{
  m_isPicked = true;

  if (m_pickable != nullptr)
  {
    const FString picktype = m_pickable->GetPickType();
    if (picktype == TEXT("ConsumeItem"))
    {
      MLibrary::UE::Audio::AudioKit::PlaySE(TEXT("Get_HErb"));
    }
    else if(picktype == TEXT("UpgradeMaterial"))
    {
      MLibrary::UE::Audio::AudioKit::PlaySE(TEXT("Get_Item"));
    }
  }
}

bool USmithPickUpItemEvent::IsDisposed() const
{
  return m_isPicked;
}

void USmithPickUpItemEvent::AssignPickable(IPickable* pickable, UNiagaraSystem* itemEventNiagara)
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

  m_isPicked = false;

  m_itemEventNiagaraSystem = itemEventNiagara;
  
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
