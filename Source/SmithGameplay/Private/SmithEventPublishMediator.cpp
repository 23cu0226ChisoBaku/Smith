// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEventPublishMediator.h"

#include "SmithMapSystem.h"
#include "SmithEventPublisher.h"
#include "SmithPickUpItemEvent.h"
#include "NiagaraSystem.h"

#include "MLibrary.h"

USmithEventPublishMediator::USmithEventPublishMediator(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_eventPublisher(nullptr)
  , m_mapSys(nullptr)
{ }

void USmithEventPublishMediator::BeginDestroy()
{
  m_mapSys.Reset();
  m_eventPublisher.Reset();

  Super::BeginDestroy();
}

void USmithEventPublishMediator::Initialize(USmithEventPublisher* eventPublisher, TSharedPtr<UE::Smith::Map::FSmithMapSystem> mapSys)
{
  m_eventPublisher = eventPublisher;
  m_mapSys = mapSys;
}

void USmithEventPublishMediator::PublishPickUpEvent(AActor* mapObj, IPickable* pickable)
{
  check(mapObj != nullptr);
  
  if (!m_eventPublisher.IsValid())
  {
    return;
  }

  TSharedPtr<UE::Smith::Map::FSmithMapSystem> mapSys_shared = m_mapSys.Pin();
  if (!mapSys_shared.IsValid())
  {
    return;
  }

  uint8 publishCoordX = 0;
  uint8 publishCoordY = 0;
  bool success = mapSys_shared->GetMapObjectCoord(mapObj, publishCoordX, publishCoordY);
  if (success)
  {
    USmithPickUpItemEvent* pickEvent = m_eventPublisher->PublishMapEvent<USmithPickUpItemEvent>(USmithPickUpItemEvent::StaticClass());
    if (pickEvent == nullptr)
    {
      MDebug::LogError("publish event failed --- can not CREATE event!!!");
      return;
    }

    FString Path = TEXT("/Game/Resources/Effect/EventEffect/NS_ItemEffect.NS_ItemEffect");
    UNiagaraSystem* itemEffect = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr, *Path));
    pickEvent->AssignPickable(pickable, itemEffect);
    mapSys_shared->DeployEvent(pickEvent, publishCoordX, publishCoordY);
  }
  else
  {
    MDebug::LogError("Can not get map Object Coord");
  }
}
