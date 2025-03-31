// Fill out your copyright notice in the Description page of Project Settings.
/*

ObstacleInfoContainer.h

Author : MAI ZHICONG(バクチソウ)

Description : 障害物タイルの情報コンテナ

Update History: 2025/01/05 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_OBSTACLE_TILEINFO
#define SMITH_MAP_OBSTACLE_TILEINFO

#include "TileInfoContainer.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief 障害物タイル情報コンテナ
    ///	namespace UE::Smith::Map
    /// 
    class FObstacleTileInfoContainer : public FTileInfoContainer
    {
      // 親クラスの全てのコンストラクタを使えるように
      using FTileInfoContainer::FTileInfoContainer;

      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FObstacleTileInfoContainer();
        virtual ~FObstacleTileInfoContainer();

        //---------------------------------------
        /*
                        コピー
        */
        //---------------------------------------
        FObstacleTileInfoContainer(const FObstacleTileInfoContainer&);
        FObstacleTileInfoContainer& operator=(const FObstacleTileInfoContainer&);
        //---------------------------------------
        /*
                        ムーブ
        */
        //---------------------------------------
        FObstacleTileInfoContainer(FObstacleTileInfoContainer&&) noexcept;
        FObstacleTileInfoContainer& operator=(FObstacleTileInfoContainer&&) noexcept;
      private:
        ///
        /// @brief 親クラス実装オーバーライド
        ///
        virtual bool isAbleToStayOnImpl() const override;
    };
  }
}

#endif
