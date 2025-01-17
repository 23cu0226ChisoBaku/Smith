// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithChasePlayerTracker.h"
#include "SmithMapManager.h"
#include "Direction.h"
#include "ICanSetOnMap.h"
#include "MLibrary.h"

USmithChasePlayerTracker::USmithChasePlayerTracker(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mapMgr(nullptr)
  , m_targetPlayer(nullptr)
{ }

void USmithChasePlayerTracker::SetupTracker(TSharedPtr<UE::Smith::Map::FSmithMapManager> mapMgr, ICanSetOnMap* player)
{
  m_mapMgr = mapMgr;
  m_targetPlayer = player;
}

bool USmithChasePlayerTracker::TrackTarget(EDirection& outDirection, ICanSetOnMap* chaser, uint8 chaseRadius)
{
  using namespace UE::Smith::Map;

  if (!m_mapMgr.IsValid() || !m_targetPlayer.IsValid())
  {
    return false;
  }

  if (!IS_UINTERFACE_VALID(chaser))
  {
    return false;
  }

  TSharedPtr<FSmithMapManager> mapMgr_shared = m_mapMgr.Pin();
  return false;
}



