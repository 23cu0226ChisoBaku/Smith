// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapEventDirector.h

Author : MAI ZHICONG(バクチソウ)

Description : マップイベントを配置する前に向きなど情報を調整するクラス

Update History: 2025/02/04 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_EVENT_DIRECTOR
#define SMITH_MAP_EVENT_DIRECTOR

enum class EMapDeployRule : uint8;

namespace UE::Smith
{
  namespace Map
  {
    //---------------------------------------
    /*
                      前方宣言
    */
    //---------------------------------------
    struct FSmithMapDataModel;

    ///
    ///	@brief イベントをマップに配置する時見た目（座標や回転など）を決めるクラス
    /// namespace UE::Smith::Map
    ///
    class FSmithMapEventDirector
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMapEventDirector();
        ~FSmithMapEventDirector();
        //---------------------------------------
        /*
                        ムーブ
        */
        //---------------------------------------
        FSmithMapEventDirector(FSmithMapEventDirector&&);
        FSmithMapEventDirector& operator=(FSmithMapEventDirector&&);

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------
      private:
        FSmithMapEventDirector(const FSmithMapEventDirector&) = delete;
        FSmithMapEventDirector& operator=(const FSmithMapEventDirector&) = delete;

      public:
        /// 
        /// @brief                          マップモデルを登録する
        /// @param FSmithMapDataModel       マップモデル
        ///
        void AssignMapDataModel(TSharedPtr<FSmithMapDataModel>);
        ///
        /// @brief                          次のステージにいくイベントの座標を決める
        /// @param outX(OUT)                X座標
        /// @param outY(OUT)                Y座標
        ///
        void DirectNextLevelEventCoord(uint8& outX, uint8& outY);
        bool GetDeployableCoord(EMapDeployRule, uint8& outX, uint8& outY);
      private:
        /// @brief 実装クラス
        class EventDirectorImpl;
        TUniquePtr<EventDirectorImpl> m_pImpl;
    };
  }
}

#endif