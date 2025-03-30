// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithRoom.h

Author : MAI ZHICONG(バクチソウ)

Description : Smith部屋（不思議ダンジョン風）

Update History: 2024/12/29 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_ROOM
#define SMIHT_ROOM

namespace UE::Smith
{
  namespace Map
  {
    //---------------------------------------
    /*
                      前方宣言
    */
    //---------------------------------------
    class FSmithRect;

    ///
    /// @brief 部屋クラス
    /// namespace UE::Smith::Map
    ///　
    class FSmithRoom
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        ///
        /// @brief             部屋コンストラクタ
        /// @param sectionIdx  部屋のあるセクションのインデックス
        /// 
        FSmithRoom(uint8 sectionIdx);
        ~FSmithRoom();

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------
      private:
        FSmithRoom(const FSmithRoom&) = delete;
        FSmithRoom& operator=(const FSmithRoom&) = delete;

      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithRoom Interface
      #pragma region FSmithRoom Interface
      public:
        ///
        /// @brief             部屋を作成する
        /// @param width       部屋の横幅
        /// @param height      部屋の縦幅
        /// @param data        部屋の初期データ
        ///
        void GenerateRoom(uint8 width, uint8 height, uint8 data);
        ///
        /// @brief             部屋の横幅を取得
        /// @return            横幅
        ///
        uint8 GetWidth() const;
        ///
        /// @brief             部屋の縦幅を取得
        /// @return            縦幅
        ///
        uint8 GetHeight() const;
        ///
        /// @brief             部屋のあるセクションインデックスを取得
        /// @return            セクションインデックス
        ///
        uint8 GetSectionIdx() const;
        ///
        /// @brief             部屋の矩形を取得
        /// @return            部屋のRect
        ///
        FSmithRect GetRoomRect() const;
      #pragma endregion FSmithRoom Interface
      // end of FSmithRoom Interface
      private:
        ///
        /// @brief 実装クラス(pImplイディオム)
        ///
        class RoomImpl;
        TUniquePtr<RoomImpl> m_pImpl;
    };
  }
}

#endif