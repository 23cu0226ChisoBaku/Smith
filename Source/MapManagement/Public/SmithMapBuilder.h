// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapBuilder.h

Author : MAI ZHICONG

Description : マップを作成するクラス

Update History: 2024/12/31 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_MAP_BUILDER
#define SMITH_MAP_BUILDER

#include "CoreMinimal.h"

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

		///
		/// @brief マップ作成クラス
		///
		class MAPMANAGEMENT_API FSmithMapBuilder
		{
			//---------------------------------------
			/*
											ctorとdtor
			*/
			//---------------------------------------
			public:
				FSmithMapBuilder();
				~FSmithMapBuilder();
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
				bool Build(FSmithMap*, FSmithMapBluePrint);

		};
	}
}

#endif
