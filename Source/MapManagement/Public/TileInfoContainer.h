// Fill out your copyright notice in the Description page of Project Settings.
/*

TileInfoContainer.h

Author : MAI ZHICONG(バクチソウ)

Description : マップタイルの情報コンテナ

Update History: 2025/01/05 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#pragma once

#ifndef TILE_INFO_CONTAINER
#define TILE_INFO_CONTAINER

#include "TileType.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// ※抽象クラス
    /// @brief タイル情報コンテナ
    ///
    class FTileInfoContainer
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        ///
        /// @brief コンストラクタ
        /// @param タイルタイル
        /// 
        explicit FTileInfoContainer(ETileType);
        virtual ~FTileInfoContainer() = 0 {};
        FTileInfoContainer(const FTileInfoContainer&) noexcept;
        FTileInfoContainer& operator=(const FTileInfoContainer&) noexcept;
        FTileInfoContainer(FTileInfoContainer&&) noexcept;
        FTileInfoContainer& operator=(FTileInfoContainer&&) noexcept;
      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      public:
        ///
        /// @brief 			タイルタイプを取得
        /// @return 		ETileType
        ///
        ETileType GetTileType() const;

        void SetTileActor(AActor*);
        AActor* GetTileActor() const;

        ///
        /// @brief 			タイルにものを置けるか
        /// @return 		置ける -> true 置けない -> false
        ///
        bool IsAbleToStayOn() const;
        /// 
        /// @brief			タイル情報をリセットする
        ///
        void Reset();
      private:
        ///
        /// @brief			IsAbleToStayOn()実装(仮想関数)
        /// @return			置ける -> true 置けない -> false
        ///
        virtual bool isAbleToStayOnImpl() const;
        /// 
        /// @brief			Reset()実装(仮想関数)
        /// 
        virtual void resetImpl();
      private:
        ///
        /// @brief      タイルタイプ
        ///
        ETileType m_tileType;
        TWeakObjectPtr<AActor> m_tileActor;
    };

    inline ETileType FTileInfoContainer::GetTileType() const
    {
      return m_tileType;
    }
    inline void FTileInfoContainer::SetTileActor(AActor* actor)
    {
      m_tileActor = actor;
    }
    inline AActor* FTileInfoContainer::GetTileActor() const
    {
      return m_tileActor.Get();
    }
  }
}

#endif
