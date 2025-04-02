// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMap.h

Author : MAI ZHICONG(バクチソウ)

Description : マップクラス

Update History: 2024/12/31 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP
#define SMITH_MAP

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
    class FSmithSection;

    /// 
    /// @brief マップクラス 
    ///
    class FSmithMap
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMap();
        ~FSmithMap();

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------				
      private:
        FSmithMap(const FSmithMap&) = delete;
        FSmithMap& operator=(const FSmithMap&) = delete;

      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithMap Interface
      #pragma region FSmithMap Interface
        public:
          ///
          /// @brief                  マップを生成する（セクションは均等で生成される）
          /// @param row              マップにあるセクションの行数
          /// @param column           マップにあるセクションの列数
          /// @param widthPerSection  セクションの横幅
          /// @param heightPerSection セクションの縦幅
          /// @param sectionGap       セクションの間のギャップ
          /// @param defaultValue     マップのマスのデータ
          ///
          void GenerateMap(uint8 row, uint8 column, uint8 widthPerSection,uint8 heightPerSection, uint8 sectionGap, uint8 mapDefaultValue);
          ///
          /// @brief                  部屋を生成する
          /// @param sectionIdx       生成するセクションのインデックス
          /// @param roomMinWidth     部屋の最小横幅
          /// @param roomMaxWidth     部屋の最大横幅
          /// @param roomMinHeight    部屋の最小縦幅
          /// @param roomMaxHeight    部屋の最大縦幅
          /// @param defaultValue     部屋のデータ
          ///
          void GenerateRoom(uint8 sectionIdx, uint8 roomMinWidth, uint8 roomMaxWidth, uint8 roomMinHeight, uint8 roomMaxHeight, uint8 roomDefaultValue);
          ///
          /// @brief                  部屋を接続する
          /// @param pathwayData      通路のデータ
          ///
          void ConnectRooms(uint8 pathwayData);
          ///
          /// @brief                  セクションの数を取得
          /// @return                 セクションの数
          ///
          int32 GetSectionCount() const;
          ///
          /// @brief                  セクションの行数を取得
          /// @return                 セクションの行数
          ///
          uint8 GetRow() const;
          ///
          /// @brief                  セクションの列数を取得
          /// @return                 セクションの列数
          ///
          uint8 GetColumn() const;
          ///
          /// @brief                  マップの矩形を取得
          /// @return                 マップの矩形
          ///
          FSmithRect GetMap() const;
          ///
          /// @brief                  セクションを取得
          /// @param rowIdx           行インデックス
          /// @param columnIdx        列インデックス
          /// @return                 見つかったらセクションへのポインタを返す 見つからなかったら　nullptr　を返す
          ///
          FSmithSection* GetSection(uint8 rowIdx, uint8 columnIdx) const;

          ///
          /// @brief                  座標でセクションを取得
          /// @param x                x座標
          /// @param y                y座標
          /// @return                 見つかったらセクションへのポインタを返す 見つからなかったら　nullptr　を返す
          ///
          FSmithSection* GetSectionByCoord(uint8 x, uint8 y) const;
          /// 
          /// @brief                  (N-1)列インデックスでN列目セクションの左座標を取得
          /// @param                  列のインデックス(均等に生成するため)
          /// @return                 セクションの左座標
          ///
          uint8 GetSectionLeft(uint8 columnIdx) const;
          /// 
          /// @brief                  (N-1)行インデックスでN行目セクションの上座標を取得
          /// @param                  行のインデックス(均等に生成するため)
          /// @return                 セクションの上座標
          ///
          uint8 GetSectionTop(uint8 rowIdx) const;
          ///
          /// @brief                  マップの横幅を取得
          /// @return                 横幅
          ///
          uint8 GetMapWidth() const;
          ///
          /// @brief                  マップの高さを取得
          /// @return                 高さ
          ///
          uint8 GetMapHeight() const;
      #pragma endregion FSmithMap Interface
      // end of FSmithMap Interface
      private:
        ///
        /// @brief 実装クラス(pImplイディオム)
        ///
        class MapImpl;
        TUniquePtr<MapImpl> m_pImpl;
    };
  }
}

#endif