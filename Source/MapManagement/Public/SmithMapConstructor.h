// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapConstructor.h

Author : MAI ZHICONG(バクチソウ)

Description : マップオブジェクトをワールドに配置するクラス

Update History: 2025/01/04 作成
                2025/01/10 マップタイルアクターを破棄するインターフェース追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_CONSTRUCTOR
#define SMITH_MAP_CONSTRUCTOR

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
struct FSmithMapConstructionBluePrint;

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
    class FSmithMap;

    ///
    /// @brief マップオブジェクトを配置するクラス
    /// namespace UE::Smith::Map
    ///
    class FSmithMapConstructor
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMapConstructor() = default;
        ~FSmithMapConstructor() = default;

      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithMapConstructor Interface
      #pragma region FSmithMapConstructor Interface
      public:
        ///
        ///	@brief				マップを構築する
        /// @param				Unreal Engine ワールド
        /// @param				マップの矩形
        /// @param				マップ構築設計図
        ///
        void ConstructMap(UWorld*, const FSmithRect&, const FSmithMapConstructionBluePrint&);
        ///
        ///	@brief				マップタイルアクターを破棄する
        ///
        void ConstructDecoration(UWorld*, FSmithMap*, TSubclassOf<AActor>, int32 count, TArray<AActor*>& outDecorations);
        void DestructMap();
        TArray<AActor*> GetMapMaterials() const; 
      #pragma endregion FSmithMapConstructor Interface
      // end of FSmithMapConstructor Interface
      private:
        /// @brief マップを構成するタイルアクターのコンテナ
        TArray<TWeakObjectPtr<AActor>> m_mapMaterials;
        TArray<TWeakObjectPtr<AActor>> m_mapDecorations;
    };
  }
}

#endif