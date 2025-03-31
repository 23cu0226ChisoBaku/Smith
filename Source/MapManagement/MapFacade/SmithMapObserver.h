// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObserver.h

Author : MAI ZHICONG(バクチソウ)

Description : マップを観察し処理するクラス(※Smithマップ専用)

Update History: 2025/01/07 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_OBSERVER
#define SMITH_MAP_OBSERVER

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
struct FSmithEnemyGenerateBluePrint;
struct FMapCoord;
enum class EDirection : uint8;

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

    class FSmithMapObserver
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------
      public:
        FSmithMapObserver();
        ~FSmithMapObserver();
        //---------------------------------------
        /*
                          ムーブ
        */
        //---------------------------------------
        FSmithMapObserver(FSmithMapObserver&&) noexcept;
        FSmithMapObserver& operator=(FSmithMapObserver&&) noexcept;

      //---------------------------------------
      /*
                      コピー禁止
      */
      //---------------------------------------
      private:
        FSmithMapObserver(const FSmithMapObserver&) = delete;
        FSmithMapObserver& operator=(const FSmithMapObserver&) = delete;

      //---------------------------------------
      /*
              パブリック関数(インターフェース)
      */
      //---------------------------------------
      // FSmithMapObserver Interface
      #pragma region FSmithMapObserver Interface
      public:
        ///
        ///	@brief													マップモデルを登録する
        /// @param	FSmithMapDataModel			マップモデル
        ///
        void AssignMapDataModel(TSharedPtr<FSmithMapDataModel>);
        ///
        ///	@brief																マップオブジェクトを初期化する
        /// @param	outMapObjs										マップ座標と座標に配置するオブジェクトコンテナ
        /// @param	UWorld												Unreal Engine ワールド
        /// @param	player												プレイヤーポインタ
        /// @param	FSmithEnemyGenerateBluePrint	敵を生成する設計図
        ///
        void InitMapObj(TMap<FMapCoord, AActor*>& outMapObjs, UWorld*, AActor* player, const FSmithEnemyGenerateBluePrint&);
        void ClearMapObjs_IgnorePlayer();
        bool ChasePlayer(EDirection& outChaseDirection, AActor* chaser, uint8 chaseRadius);
        bool GetMapObjectCoord(AActor*, uint8& outX, uint8& outY);
        /// @brief 未使用
        void GenerateNewEnemy();
        /// @brief 未使用
        bool ChaseTarget(EDirection& outChaseDirection, AActor* chaser, AActor* target, uint8 chaseRadius);
        /// TODO
        bool ConvertMapCoordToWorldLocation(FVector& outLocation, uint8 x, uint8 y);
        // TODO
        void GetPlayerCoord(FMapCoord& outPlayerCoord);

      #pragma endregion FSmithMapObserver Interface
      // end of FSmithMapObserver Interface
      private:
        ///
        ///	@brief FSmithMapObserver実装クラス(pImplイディオム)
        ///
        class MapObserverImpl;
        TUniquePtr<MapObserverImpl> m_pImpl;
    };
  }
}

#endif