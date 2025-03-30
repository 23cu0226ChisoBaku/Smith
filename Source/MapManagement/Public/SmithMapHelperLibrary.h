// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapHelperLibrary.h

Author : MAI ZHICONG(バクチソウ)

Description : FSmithMapヘルパーライブラリ

Update History: 2025/01/15 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_HELPER_LIB
#define SMITH_MAP_HELPER_LIB

enum class EMapDeployRule : uint8;
struct FMapCoord;

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

    ///
    /// @brief FSmithMapヘルパー関数ライブラリ
    ///	namespace UE::Smith::Map
    ///
    class FSmithMapHelperLibrary
    {
      // インスタンス作成禁止
      private:
        FSmithMapHelperLibrary() = delete;
        ~FSmithMapHelperLibrary() = delete;

      public:
        ///
        /// @brief            二つの座標が同じ部屋にあるか
        /// @param FSmithMap  マップ
        /// @param x1         一つ目のX座標
        /// @param y1         一つ目のY座標
        /// @param x2         二つ目のX座標
        /// @param y2         二つ目のY座標
        /// @return           同じだった        ->  true
        ///                   同じじゃなかった  ->  false
        ///
        static bool IsInSameRoom(FSmithMap*, uint8 x1, uint8 y1, uint8 x2, uint8 y2);
        ///
        /// @brief            二つの座標が同じセクションにあるか
        /// @param FSmithMap	マップ 					
        /// @param x1 				一つ目のX座標
        /// @param y1         一つ目のY座標
        /// @param x2         二つ目のX座標
        /// @param y2         二つ目のY座標
        /// @return           同じだった        ->  true
        ///                   同じじゃなかった  ->  false
        ///
        static bool IsInSameSection(FSmithMap*, uint8 x1, uint8 y1, uint8 x2, uint8 y2);
        ///
        /// @brief                  マップにある要素の向きを決める
        /// @param FSmithMap        マップ
        /// @param outRotation(OUT) UEワールド回転					
        /// @param x                X座標
        /// @param y                Y座標
        ///
        static void DirectMapElementRotation(FSmithMap*, FRotator& outRotation, uint8 x, uint8 y);
        static int32 GetMapCoordsByRule(FSmithMap*, EMapDeployRule, TArray<FMapCoord>& outCoords);
        static uint8 GetRoomCount(FSmithMap*);
        static bool IsSameTileNearby(FSmithMap*, const FMapCoord&, FMapCoord& outDiffTileCoord);
    };
  }
}

#endif