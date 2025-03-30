// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapDataModel.cpp

Author : MAI ZHICONG(バクチソウ)

Description : マップデータ情報モデル

Update History: 2025/01/06 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapDataModel.h"

#include "InvalidValues.h"
#include "SmithMap.h"
#include "ISmithMapModelRequester.h"

namespace UE::Smith
{
  namespace Map
  {
    FSmithMapDataModel::FSmithMapDataModel()
      : Map(nullptr)
      , OnMapObjsCoordTable{}
      , ObstacleTable{}
      , StaySpaceTable{}
      , OriginWorldCoord(FVector::ZeroVector)
      , MapTileSize(0)
      , MapModelRequester(nullptr)
    { }

    FSmithMapDataModel::~FSmithMapDataModel()
    {
      Map.Reset();
      OnMapObjsCoordTable.Empty();
      ObstacleTable.Empty();
      StaySpaceTable.Empty();
      MapModelRequester.Reset();
    }

    FSmithMapDataModel::FSmithMapDataModel(FSmithMapDataModel&& other) noexcept
      : Map(::MoveTemp(other.Map))
      , OnMapObjsCoordTable(::MoveTemp(other.OnMapObjsCoordTable))
      , ObstacleTable(::MoveTemp(other.ObstacleTable))
      , StaySpaceTable(::MoveTemp(other.StaySpaceTable))
      , OriginWorldCoord(::MoveTemp(other.OriginWorldCoord))
      , MapTileSize(other.MapTileSize)
      , MapModelRequester(other.MapModelRequester)
    { }

    FSmithMapDataModel& FSmithMapDataModel::operator=(FSmithMapDataModel&& other) noexcept
    {
      if (this != &other)
      {
        Map = ::MoveTemp(other.Map);
        OnMapObjsCoordTable = ::MoveTemp(other.OnMapObjsCoordTable);
        ObstacleTable = ::MoveTemp(other.ObstacleTable);
        StaySpaceTable = ::MoveTemp(other.StaySpaceTable);
        OriginWorldCoord = ::MoveTemp(other.OriginWorldCoord);
        MapTileSize = other.MapTileSize;
        MapModelRequester = ::MoveTemp(other.MapModelRequester);
      }

      return *this;
    }
  }
}

// #if UE_BUILD_DEBUG
// uint32 GetTypeHash(const TWeakInterfacePtr<ICanSetOnMap>& Thing)
// {
//   uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(TWeakInterfacePtr<ICanSetOnMap>));
//   return Hash;
// }
// #endif
