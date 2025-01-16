// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapManager.h

Author : MAI ZHICONG

Description : マップマネージャー

Update History: 2025/01/04 作成
              : 2025/01/06 クラス分けてマップを操作する

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_MGR
#define SMITH_MAP_MGR

#include "CoreMinimal.h"

//---------------------------------------
/*
									前方宣言
*/
//---------------------------------------
class IAttackable;
class ICanSetOnMap;
namespace UE::Smith
{
	namespace Battle
	{
    class FSmithCommandFormat;
  }
}

enum class EDirection : uint8;

struct FMapCoord;
struct FSmithMapBluePrint;
struct FSmithMapConstructionBluePrint;
struct FSmithEnemyGenerateBluePrint;

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
		///	@brief マップを管理するマネージャー
		/// namespace UE::Smith::Map
		///
    class MAPMANAGEMENT_API FSmithMapManager final
    {			
      //---------------------------------------
			/*
											ctorとdtor
			*/
			//---------------------------------------
      public:
        FSmithMapManager();
        ~FSmithMapManager();
        //---------------------------------------
        /*
                          ムーブ
        */
        //---------------------------------------
        FSmithMapManager(FSmithMapManager&&) noexcept;
        FSmithMapManager& operator=(FSmithMapManager&&) noexcept;

      //---------------------------------------
			/*
											コピー禁止
			*/
			//---------------------------------------
      private:
        FSmithMapManager(const FSmithMapManager&) = delete;
        FSmithMapManager& operator=(const FSmithMapManager&) = delete;
      
      //---------------------------------------
			/*
							パブリック関数(インターフェース)
			*/
			//---------------------------------------
			// FSmithMapManager Interface
			#pragma region FSmithMapManager Interface
      public:
        ///
        /// @brief                                マップを初期化する
        /// @param UWorld                         ダンジョンオブジェクトを配置するUnreal Engine ワールド
        /// @param FSmithMapBluePrint             ダンジョンマップ設計図
        /// @param FSmithMapConstructionBluePrint ダンジョンマップ構築用設計図
        ///
        void InitMap(UWorld*, const FSmithMapBluePrint&, const FSmithMapConstructionBluePrint&);
        ///
        /// @brief                                マップオブジェクトを生成する
        /// @param UWorld                         マップオブジェクトを配置するUnreal Engine ワールド
        /// @param player                         プレイヤーポインタ
        /// @param FSmithEnemyGenerateBluePrint   敵生成する設計図
        ///
        void InitMapObjs(UWorld*, AActor* player, const FSmithEnemyGenerateBluePrint&);
        /// 
        /// @brief                                マップオブジェクトを配置する
        /// @param ICanSetOnMap                   マップオブジェクト
        /// @param x                              X座標
        /// @param y                              Y座標
        ///
        void DeployMapObj(ICanSetOnMap*, uint8 x, uint8 y);
        ///
        /// @brief                                攻撃できるオブジェクトを探す                      
        /// @param outActors                      攻撃できるオブジェクトコンテナ(OUT)
        /// @param ICanSetOnMap                   攻撃を行うマップオブジェクト
        /// @param FSmithCommandFormat            攻撃フォーマット
        ///
        void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap*, const UE::Smith::Battle::FSmithCommandFormat&);
        ///
        /// @brief                                マップオブジェクトを移動
        /// @param ICanSetOnMap                   マップオブジェクト
        /// @param EMoveDirection                 移動方向
        /// @param moveDistance                   移動距離(何マス分)
        /// @param FVector                        移動先の座標
        ///
        void MoveMapObj(ICanSetOnMap*, EDirection, uint8 moveDistance, FVector&);
			#pragma endregion FSmithMapManager Interface
			// end of FSmithMapManager Interface
      private:
        ///
        /// @brief マップマネージャー実装クラス(pImplイディオム)
        ///
        class MapMgrImpl;
        TUniquePtr<MapMgrImpl> m_pImpl;
    };
  }
}

#endif