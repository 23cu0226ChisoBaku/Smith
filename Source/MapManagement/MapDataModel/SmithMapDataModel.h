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

#include "UObject/WeakInterfacePtr.h"
#include "MapCoord.h"
#include "ObstacleTileInfoContainer.h"
#include "StaySpaceTileInfoContainer.h"


class FObstacleTileInfoContainer;
class FStaySpaceTileInfoContainer;

// TODO
class ISmithMapModelRequester;

namespace UE::Smith
{
  namespace Map
  {
    class FSmithMap;
    /// 
    /// @brief マップデータモデル
    /// namespace UE::Smith::Map
    ///
    struct FSmithMapDataModel
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
        TMap<TWeakObjectPtr<AActor>, FMapCoord> OnMapObjsCoordTable;
        TMap<FMapCoord, TSharedPtr<FObstacleTileInfoContainer>> ObstacleTable;
        TMap<FMapCoord, TSharedPtr<FStaySpaceTileInfoContainer>> StaySpaceTable;
        FVector OriginWorldCoord;
        int32 MapTileSize;

        // TODO need make these models private
        // TODO Use Presenter
        TWeakInterfacePtr<ISmithMapModelRequester> MapModelRequester;
    };
  }
}

