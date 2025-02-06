// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEventPublishMediator.h"
#include "SmithMapManager.h"
#include "SmithEventPublisher.h"
#include "ICanSetOnMap.h"
#include "SmithPickable.h"
#include "SmithPickUpItemEvent.h"
#include "NiagaraSystem.h"

USmithEventPublishMediator::USmithEventPublishMediator(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_eventPublisher(nullptr)
  , m_mapMgr(nullptr)
{ }

void USmithEventPublishMediator::BeginDestroy()
{
  m_mapMgr.Reset();
  m_eventPublisher.Reset();

  Super::BeginDestroy();
}

void USmithEventPublishMediator::Initialize(USmithEventPublisher* eventPublisher, TSharedPtr<UE::Smith::Map::FSmithMapManager> mapMgr)
{
  m_eventPublisher = eventPublisher;
  m_mapMgr = mapMgr;
}

void USmithEventPublishMediator::PublishPickUpEvent(ICanSetOnMap* mapObj, USmithPickable* pickable)
{
  check(m_eventPublisher.IsValid());
  if (!m_eventPublisher.IsValid())
  {
    return;
  }

  TSharedPtr<UE::Smith::Map::FSmithMapManager> mgr_shared = m_mapMgr.Pin();
  if (!mgr_shared.IsValid())
  {
    return;
  }

  uint8 publishCoordX = 0;
  uint8 publishCoordY = 0;
  bool success = mgr_shared->GetMapObjectCoord(mapObj,publishCoordX, publishCoordY);
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
    mgr_shared->DeployEvent(pickEvent, publishCoordX, publishCoordY);
  }
  else
  {
    MDebug::LogError("Can not get map Object Coord");
  }
}
