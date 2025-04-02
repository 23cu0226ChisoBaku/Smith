// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithChasePlayerTracker.h"

#include "SmithMapSystem.h"
#include "Direction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithChasePlayerTracker)

USmithChasePlayerTracker::USmithChasePlayerTracker(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_mapSys(nullptr)
{ }

void USmithChasePlayerTracker::SetupTracker(const TSharedPtr<UE::Smith::Map::FSmithMapSystem>& mapSys)
{
  m_mapSys = mapSys;
}

bool USmithChasePlayerTracker::TrackTarget(EDirection& outDirection, AActor* Chaser, uint8 chaseRadius)
{
  check(Chaser != nullptr);

  TSharedPtr<UE::Smith::Map::FSmithMapSystem> mapSys_shared = m_mapSys.Pin();
  if (!mapSys_shared.IsValid())
  {
    return false;
  }

  return mapSys_shared->ChasePlayerTarget(outDirection, Chaser, chaseRadius);
}



