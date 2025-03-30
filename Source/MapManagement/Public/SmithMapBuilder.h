// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapBuilder.h

Author : MAI ZHICONG(バクチソウ)

Description : マップを作成するクラス

Update History: 2024/12/31 作成
              : 2024/01/05 マップモデル作成関数追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_BUILDER
#define SMITH_MAP_BUILDER

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
struct FSmithMapBluePrint;

namespace UE::Smith
{
  namespace Map
  {
    //---------------------------------------
    /*
                      前方宣言
    */
    //---------------------------------------
    class FSmithMap;
    struct FSmithMapDataModel;

    ///
    /// @brief マップ作成クラス
    /// namespace UE::Smith::Map
    ///
    class FSmithMapBuilder
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMapBuilder() = default;
        ~FSmithMapBuilder() = default;
      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      public:
        ///
        /// @brief        マップを作成
        /// @param        マップポインター
        /// @param        設計図
        /// @return       作成成功-> true 作成失敗-> false
        ///
        bool Build(TSharedPtr<FSmithMap>, const FSmithMapBluePrint&);
        ///
        /// @brief        マップモデルを作成する
        /// @param        マップポインター(TSharedPtr)
        /// @return       作成成功-> モデルへのSharedPtr 作成失敗-> nullptr
        ///
        TSharedPtr<FSmithMapDataModel> GenerateModel(TSharedPtr<FSmithMap>);

      private:
        // TODO Temp Code
        void generateRoom(TSharedPtr<FSmithMap>, const FSmithMapBluePrint& , uint8 sectionIdx);
    };
  }
}

#endif
