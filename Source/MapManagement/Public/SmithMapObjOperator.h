// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjOperator.h

Author : MAI ZHICONG

Description : マップオブジェクトを操作する(移動など)クラス

Update History: 2025/01/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_OPERATOR
#define SMITH_MAP_OPERATOR

#include "CoreMinimal.h"

//---------------------------------------
/*
									前方宣言
*/
//---------------------------------------
class IAttackable;
class ICanSetOnMap;
class IEventRegister;
enum class EDirection : uint8;
namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
	}
}

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

		class MAPMANAGEMENT_API FSmithMapObjOperator
		{
			//---------------------------------------
			/*
											ctorとdtor
			*/
			//---------------------------------------
			public:
				FSmithMapObjOperator();
				~FSmithMapObjOperator();
				//---------------------------------------
				/*
													ムーブ
				*/
				//---------------------------------------
				FSmithMapObjOperator(FSmithMapObjOperator&&) noexcept;
				FSmithMapObjOperator& operator=(FSmithMapObjOperator&&) noexcept;

			//---------------------------------------
			/*
											コピー禁止
			*/
			//---------------------------------------
			private:
				FSmithMapObjOperator(const FSmithMapObjOperator&) = delete;
				FSmithMapObjOperator& operator=(const FSmithMapObjOperator&) = delete;
			//---------------------------------------
			/*
							パブリック関数(インターフェース)
			*/
			//---------------------------------------
			// FSmithMapObserver Interface
			#pragma region FSmithMapObjOperator Interface
			public:
				void AssignEventRegister(IEventRegister*);
				///
				///	@brief													マップモデルを登録する
				/// @param	FSmithMapDataModel			マップモデル
				/// @param	originCoord_World				マップ原点座標
				/// @param	mapTileSize							マップタイルサイズ
				///
				void AssignMap(TSharedPtr<FSmithMapDataModel>);
				///
				/// @brief 													攻撃できるオブジェクトを探す
				/// @param outActors 								攻撃できるオブジェクト(IAttackable)が全て入っているコンテナ
				/// @param ICanSetOnMap							ターゲット
				/// @param FSmithCommandFormat			攻撃フォーマット
				///
				void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap*, const UE::Smith::Battle::FSmithCommandFormat&);
				///
				/// @brief 													オブジェクトを移動する
				/// @param ICanSetOnMap							移動するターゲットオブジェクト
				/// @param EMoveDirection						移動方向
				/// @param moveDistance 						移動距離
				/// @param FVector									移動先の座標
				///
				void MoveMapObj(ICanSetOnMap*, EDirection, uint8 moveDistance, FVector&);
			#pragma endregion FSmithMapObjOperator Interface
			// end of FSmithMapObjOperator Interface
			private:
				///
				///	@brief FSmithMapObjOperator実装クラス(pImplイディオム)
				///
				class MapObjOperatorImpl;
				TUniquePtr<MapObjOperatorImpl> m_pImpl;
		};
	}
}

#endif
