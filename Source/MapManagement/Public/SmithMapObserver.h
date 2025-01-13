// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


class ICanSetOnMap;
struct FSmithEnemyGenerateBluePrint;
struct FMapCoord;

namespace UE::Smith
{
	namespace Map
	{
		struct FSmithMapDataModel;

		class MAPMANAGEMENT_API FSmithMapObserver
		{
			public:
				FSmithMapObserver();
				~FSmithMapObserver();
				FSmithMapObserver(FSmithMapObserver&&) noexcept;
				FSmithMapObserver& operator=(FSmithMapObserver&&) noexcept;

			private:
				FSmithMapObserver(const FSmithMapObserver&) = delete;
				FSmithMapObserver& operator=(const FSmithMapObserver&) = delete;

			public:
			  void AssignMap(TSharedPtr<FSmithMapDataModel>, FVector originCoord_World, int32 mapTileSize);
				void InitMapObj(TMap<FMapCoord, ICanSetOnMap*>& outMapObjs, UWorld*, AActor* player, const FSmithEnemyGenerateBluePrint&);
				void GenerateNewEnemy();
				void ChaseTarget(ICanSetOnMap* chaser, ICanSetOnMap* target, uint8 chaseRadius);

			private:
				class MapObserverImpl;
				TUniquePtr<MapObserverImpl> m_pImpl;
		};
	}
}
