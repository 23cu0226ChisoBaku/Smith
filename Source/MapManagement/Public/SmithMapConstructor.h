// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapConstructor.h

Author : MAI ZHICONG

Description : マップオブジェクトをワールドに配置するクラス

Update History: 2024/01/04 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_CONSTRUCTOR
#define SMITH_MAP_CONSTRUCTOR

#include "CoreMinimal.h"
//---------------------------------------
/*
									前方宣言
*/
//---------------------------------------
struct FSmithMapConstructionBluePrint;
enum class ETileType : uint8;

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
		/// @brief マップオブジェクトを配置するクラス
		/// namespace UE::Smith::Map
		///
		class MAPMANAGEMENT_API FSmithMapConstructor
		{
			//---------------------------------------
			/*
											ctorとdtor
			*/
			//---------------------------------------
			public:
				FSmithMapConstructor();
				~FSmithMapConstructor();

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
			#pragma endregion FSmithMapConstructor Interface
			// end of FSmithMapConstructor Interface
		};
	}
}

#endif