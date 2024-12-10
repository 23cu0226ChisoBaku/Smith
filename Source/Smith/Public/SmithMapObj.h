// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MAPOBJ
#define SMITH_MAPOBJ

#include "CoreMinimal.h"

/*

Smith Map Object

Author : MAI ZHICONG

Description : Map Object(Use by MapManager)

Update History: 2024/11/25 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/

namespace Smith
{
	class SMITH_API FSmithMapObj
	{
	public:
		FSmithMapObj(AActor* const);
		virtual ~FSmithMapObj();

	public:
		/// @brief ユニークIDを返す
		/// @return FGuid
		FGuid GetUniqueID() const;	
		/// @brief 管理するAActorのポインタを返す
		/// @return AActor* (AActorが消えたらnullptrを返す)
		AActor* GetActor() const;
		/// @brief 有効性チェック
		/// @return 有効性
		bool IsValid() const;
    /// @brief 同じAActorを参照しているかをチェック
		/// @return 同じだったらtrue, 引数が無効もしくは同じじゃなかったらfalse
		bool ReferenceEquals(AActor* const) const;

	private:
		FGuid m_uniqueID;
		TWeakObjectPtr<AActor> m_actor;
	};
}

#endif