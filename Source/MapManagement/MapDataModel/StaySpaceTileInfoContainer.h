// Fill out your copyright notice in the Description page of Project Settings.
/*

StaySpaceTileInfoContainer.h

Author : MAI ZHICONG(バクチソウ)

Description : 

Update History: 2025/01/05 作成
                2025/02/04 インターフェース修正   SetPickable、GetPickable、CanPlacePickable削除
                                                SetEvent、GetEvent追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_STAYSPACE_TILEINFO
#define SMITH_MAP_STAYSPACE_TILEINFO

#include "TileInfoContainer.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class ISmithMapEvent;

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief ものやキャラクターを置けるタイル情報コンテナ
    /// namespace UE::Smith::Map
    ///
    class FStaySpaceTileInfoContainer : public FTileInfoContainer
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        /// @param ETileType タイルタイプ
        explicit FStaySpaceTileInfoContainer(ETileType);
        ~FStaySpaceTileInfoContainer();

        //---------------------------------------
        /*
                        コピー
        */
        //---------------------------------------
        FStaySpaceTileInfoContainer(const FStaySpaceTileInfoContainer&);
        FStaySpaceTileInfoContainer& operator=(const FStaySpaceTileInfoContainer&);

        //---------------------------------------
        /*
                        ムーブ
        */
        //---------------------------------------
        FStaySpaceTileInfoContainer(FStaySpaceTileInfoContainer&&) noexcept;
        FStaySpaceTileInfoContainer& operator=(FStaySpaceTileInfoContainer&&) noexcept;

      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FStaySpaceTileInfoContainer Interface
      #pragma region FStaySpaceTileInfoContainer Interface
      public:
        ///
        /// @brief マップオブジェクトを設定する
        /// @param AActor マップに配置できるオブジェクト
        ///
        void SetMapObj(AActor*);
        ///
        /// @brief マップイベントを設定する
        /// @param ICanSetOnMap マップに配置できるイベント
        ///
        void SetEvent(ISmithMapEvent*);
        ///
        /// @brief マップオブジェクトを返す
        /// @return 何かがいたら	-> ここにいるマップオブジェクトのポインタ
        ///					何もいなかったら	-> nullptr
        ///
        AActor* GetMapObject() const;
        ///
        /// @brief	マップイベントを取得 
        /// @return	あったら　	->	イベントポインタ
        ///					なかったら	->	nullptr	
        /// 
        ISmithMapEvent* GetEvent() const;
      #pragma endregion FStaySpaceTileInfoContainer Interface
      // end of FStaySpaceTileInfoContainer Interface

      //---------------------------------------
      /*
                    プライベート関数
      */
      //---------------------------------------
      // FTileInfoContainer オーバーライド
      #pragma region FTileInfoContainer Override
      private:
        virtual bool isAbleToStayOnImpl() const override;
        virtual void resetImpl() override;
      #pragma endregion FTileInfoContainer Override
      //　end of FTileInfoContainer オーバーライド
      private:
        /// @brief FStaySpaceTileInfoContainerImpl実装(pImplイディオム)
        class StaySpaceTileInfoImpl;
        TUniquePtr<StaySpaceTileInfoImpl> m_pImpl;
    };
  }
}

#endif