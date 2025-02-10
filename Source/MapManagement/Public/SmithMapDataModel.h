// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapDataModel.h

Author : MAI ZHICONG(バクチソウ)

Description : マップデータ情報モデル

Update History: 2025/01/06 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "ICanSetOnMap.h"
#include "MapCoord.h"
#include "ObstacleTileInfoContainer.h"
#include "StaySpaceTileInfoContainer.h"

///
/// @brief TWeakInterfacePtr<ICanSetOnMap>をキーとしてTMapに使う
///
#if UE_BUILD_DEBUG
uint32 GetTypeHash(const TWeakInterfacePtr<ICanSetOnMap>&);
#else /// @brief optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const TWeakInterfacePtr<ICanSetOnMap>& Thing)
{
  uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(TWeakInterfacePtr<ICanSetOnMap>));
  return Hash;
}
#endif

class FObstacleTileInfoContainer;
class FStaySpaceTileInfoContainer;

namespace UE::Smith
{
  namespace Map
  {
    class FSmithMap;
    /// 
    /// @brief マップデータモデル
    /// namespace UE::Smith::Map
    ///
    struct MAPMANAGEMENT_API FSmithMapDataModel
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMapDataModel();
        ~FSmithMapDataModel();
        //---------------------------------------
        /*
                          ムーブ
        */
        //---------------------------------------
        FSmithMapDataModel(FSmithMapDataModel&&) noexcept;
        FSmithMapDataModel& operator=(FSmithMapDataModel&&) noexcept;

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------
      private:
        FSmithMapDataModel(const FSmithMapDataModel&) = delete;
        FSmithMapDataModel& operator=(const FSmithMapDataModel&) = delete;
      
      public:
        TWeakPtr<FSmithMap> Map;
        TMap<TWeakInterfacePtr<ICanSetOnMap>, FMapCoord> OnMapObjsCoordTable;
        TMap<FMapCoord, TSharedPtr<FObstacleTileInfoContainer>> ObstacleTable;
        TMap<FMapCoord, TSharedPtr<FStaySpaceTileInfoContainer>> StaySpaceTable;
        FVector OriginWorldCoord;
        int32 MapTileSize;
    };
  }
}

