// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapObjOperator.h

Author : MAI ZHICONG

Description : マップオブジェクトを操作するクラス

Update History: 2025/01/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"

class IAttackable;
class ICanSetOnMap;

namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
		enum class EMoveDirection : uint8;
	}
}

namespace UE::Smith
{
	namespace Map
	{
		class FSmithMap;
		struct FSmithMapDataModel;

		class MAPMANAGEMENT_API FSmithMapObjOperator
		{
			public:
				FSmithMapObjOperator();
				~FSmithMapObjOperator();
				FSmithMapObjOperator(FSmithMapObjOperator&&) noexcept;
				FSmithMapObjOperator& operator=(FSmithMapObjOperator&&) noexcept;

			private:
				FSmithMapObjOperator(const FSmithMapObjOperator&) = delete;
				FSmithMapObjOperator& operator=(const FSmithMapObjOperator&) = delete;

			public:
				void AssignMap(TSharedPtr<FSmithMapDataModel>, FVector originCoord_World, int32 tileSize);
				void FindAttackableMapObjs(TArray<IAttackable*>& outActors, ICanSetOnMap*, const UE::Smith::Battle::FSmithCommandFormat&);
				void MoveMapObj(ICanSetOnMap*, UE::Smith::Battle::EMoveDirection, uint8 moveDistance, FVector&);

			private:
				class MapObjOperatorImpl;
				TUniquePtr<MapObjOperatorImpl> m_pImpl;
		};
	}
}
