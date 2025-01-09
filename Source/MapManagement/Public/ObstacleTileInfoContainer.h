// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileInfoContainer.h"

/**
 * 
 */

namespace UE::Smith
{
	namespace Map
	{
		class MAPMANAGEMENT_API FObstacleTileInfoContainer : public FTileInfoContainer
		{
			using FTileInfoContainer::FTileInfoContainer;

			public:
				FObstacleTileInfoContainer();
				virtual ~FObstacleTileInfoContainer();

				FObstacleTileInfoContainer(const FObstacleTileInfoContainer&);
				FObstacleTileInfoContainer& operator=(const FObstacleTileInfoContainer&);

				FObstacleTileInfoContainer(FObstacleTileInfoContainer&&) noexcept;
				FObstacleTileInfoContainer& operator=(FObstacleTileInfoContainer&&) noexcept;
			private:
				virtual bool isAbleToStayOn_Impl() const override;
				virtual void reset_Impl() override;
		};
	}
}
