// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEventPublishMediator.h"
#include "SmithMapManager.h"
#include "SmithEventPublisher.h"
#include "ICanSetOnMap.h"
#include "SmithPickable.h"
#include "SmithPickUpItemEvent.h"

USmithEventPublishMediator::USmithEventPublishMediator(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mapMgr(nullptr)
  , m_eventPublisher(nullptr)
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
  check(m_mapMgr.IsValid());
  if (!m_eventPublisher.IsValid() || !m_mapMgr.IsValid())
  {
    return;
  }

  TSharedPtr<UE::Smith::Map::FSmithMapManager> mgr_shared = m_mapMgr.Pin();

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
    // TODO 
    pickEvent->AssignPickable(pickable, nullptr);
    mgr_shared->DeployEvent(pickEvent, publishCoordX, publishCoordY);
  }
  else
  {
    MDebug::LogError("Can not get map Object Coord");
  }
}
