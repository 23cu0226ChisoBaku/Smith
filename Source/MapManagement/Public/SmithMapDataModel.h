// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "MapCoord.h"
#include "SmithMap.h"
#include "ObstacleTileInfoContainer.h"
#include "StaySpaceTileInfoContainer.h"
#include "ICanSetOnMap.h"

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const TWeakInterfacePtr<ICanSetOnMap>&);
#else /// @brief optimize by inlining in shipping and development builds
FORCEINLINE uint32 GetTypeHash(const TWeakInterfacePtr<ICanSetOnMap>& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(TWeakInterfacePtr<ICanSetOnMap>));
	return Hash;
}
#endif

namespace UE::Smith
{
	namespace Map
	{
		struct MAPMANAGEMENT_API FSmithMapDataModel
		{
			public:
				FSmithMapDataModel();
				~FSmithMapDataModel();
				FSmithMapDataModel(FSmithMapDataModel&&) noexcept;
				FSmithMapDataModel& operator=(FSmithMapDataModel&&) noexcept;

			private:
				FSmithMapDataModel(const FSmithMapDataModel&) = delete;
				FSmithMapDataModel& operator=(const FSmithMapDataModel&) = delete;
			
			public:
        TWeakPtr<FSmithMap> Map;
        TMap<TWeakInterfacePtr<ICanSetOnMap>, FMapCoord> OnMapObjsCoordTable;
        TMap<FMapCoord, TSharedPtr<FObstacleTileInfoContainer>> ObstacleTable;
        TMap<FMapCoord, TSharedPtr<FStaySpaceTileInfoContainer>> StaySpaceTable;
		};
	}
}

