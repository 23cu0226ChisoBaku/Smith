// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithChasePlayerTracker.h"
#include "SmithMapManager.h"
#include "Direction.h"
#include "ICanSetOnMap.h"
#include "MLibrary.h"

USmithChasePlayerTracker::USmithChasePlayerTracker(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mapMgr(nullptr)
{ }

void USmithChasePlayerTracker::SetupTracker(TSharedPtr<UE::Smith::Map::FSmithMapManager> mapMgr)
{
  m_mapMgr = mapMgr;
}

bool USmithChasePlayerTracker::TrackTarget(EDirection& outDirection, ICanSetOnMap* chaser, uint8 chaseRadius)
{
  if (!IS_UINTERFACE_VALID(chaser))
  {
    return false;
  }

  TSharedPtr<UE::Smith::Map::FSmithMapManager> mapMgr_shared = m_mapMgr.Pin();
  if (!mapMgr_shared.IsValid())
  {
    return false;
  }

  return mapMgr_shared->ChasePlayerTarget(outDirection, chaser, chaseRadius);
}



