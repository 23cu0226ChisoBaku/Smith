// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapDataModel.h"
#include "InvalidValues.h"
namespace UE::Smith
{
  namespace Map
  {
    FSmithMapDataModel::FSmithMapDataModel()
      : Map(nullptr)
      , OnMapObjsCoordTable{}
      , ObstacleTable{}
      , StaySpaceTable{}
    { }

    FSmithMapDataModel::~FSmithMapDataModel()
    {
      OnMapObjsCoordTable.Empty();
      ObstacleTable.Empty();
      StaySpaceTable.Empty();
    }

    FSmithMapDataModel::FSmithMapDataModel(FSmithMapDataModel&& other) noexcept
      : Map(::MoveTemp(other.Map))
      , OnMapObjsCoordTable(::MoveTemp(other.OnMapObjsCoordTable))
      , ObstacleTable(::MoveTemp(other.ObstacleTable))
      , StaySpaceTable(::MoveTemp(other.StaySpaceTable))
    { }

    FSmithMapDataModel& FSmithMapDataModel::operator=(FSmithMapDataModel&& other) noexcept
    {
      if (this != &other)
      {
        Map = ::MoveTemp(other.Map);
        OnMapObjsCoordTable = ::MoveTemp(other.OnMapObjsCoordTable);
        ObstacleTable = ::MoveTemp(other.ObstacleTable);
        StaySpaceTable = ::MoveTemp(other.StaySpaceTable);
      }

      return *this;
    }
  }
}

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const TWeakInterfacePtr<ICanSetOnMap>& Thing)
{
  uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(TWeakInterfacePtr<ICanSetOnMap>));
  return Hash;
}
#endif