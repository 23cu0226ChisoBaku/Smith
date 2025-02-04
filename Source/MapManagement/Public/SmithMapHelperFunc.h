// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjDeployDirector.h

Author : MAI ZHICONG

Description : マップオブジェクトを新しく配置するクラス

Update History: 2025/01/15 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_HELPER_FUNC
#define SMITH_MAP_HELPER_FUNC

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
    class MAPMANAGEMENT_API FSmithMapHelperFunc
    {
      // インスタンス作成禁止
      private:
        FSmithMapHelperFunc() = delete;
        ~FSmithMapHelperFunc() = delete;

      public:
        ///
        /// @brief            二つの座標が同じ部屋にあるか
        /// @param FSmithMap	マップ 					
        /// @param x1 				一つ目のX座標
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

        static bool DirectMapElementRotation(FSmithMap*, FRotator& outRotation, uint8 x, uint8 y);
    };
  }
}

#endif