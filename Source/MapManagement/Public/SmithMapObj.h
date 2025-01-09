// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Map Object

Author : MAI ZHICONG

Description : Map Object(Use by MapManager)

Update History: 2024/11/25 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#pragma once

#ifndef SMITH_MAPOBJ
#define SMITH_MAPOBJ

#include "CoreMinimal.h"

namespace UE::Smith
{
	namespace Map
	{

		class MAPMANAGEMENT_API FSmithMapObj final
		{
			public:
				explicit FSmithMapObj(AActor*);
				~FSmithMapObj();

			public:
				///
				/// @brief       ユニークIDを返す
				/// @return      FGuid
				///
				FGuid GetUniqueID() const;	
				///
				/// @brief       管理するAActorのポインタを返す
				/// @return      AActor* (AActorが消えたらnullptrを返す)
				///
				AActor* GetActor() const;
				///
				/// @brief      有効性チェック
				/// @return     FGuidが有効かつAActorが破壊されていない -> true それ以外 -> false
				///
				bool IsValid() const;
				///
				/// @brief 同じAActorを参照しているかをチェック
				/// @return 同じ -> true, 引数が無効もしくは同じじゃない -> false
				///
				bool ReferenceEquals(const AActor*) const;



			private:
				/// @brief 実装クラス
				class MapObjImpl;
				TUniquePtr<MapObjImpl> m_pImpl;
		};

		// 比較演算子
		bool operator==(const FSmithMapObj&, const FSmithMapObj&);
	}
}

#endif