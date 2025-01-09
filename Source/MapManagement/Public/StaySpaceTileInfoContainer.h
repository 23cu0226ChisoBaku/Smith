// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileInfoContainer.h"

class ICanSetOnMap;

namespace UE::Smith
{
	namespace Map
	{
		class IPickable;

		class MAPMANAGEMENT_API FStaySpaceTileInfoContainer : public FTileInfoContainer
		{
			using FTileInfoContainer::FTileInfoContainer;

			public:
				explicit FStaySpaceTileInfoContainer(ETileType);
				~FStaySpaceTileInfoContainer();

				FStaySpaceTileInfoContainer(const FStaySpaceTileInfoContainer&);
				FStaySpaceTileInfoContainer& operator=(const FStaySpaceTileInfoContainer&);

				FStaySpaceTileInfoContainer(FStaySpaceTileInfoContainer&&) noexcept;
				FStaySpaceTileInfoContainer& operator=(FStaySpaceTileInfoContainer&&) noexcept;
			public:
				void SetMapObj(ICanSetOnMap*);
				void SetPickable(IPickable*);
				ICanSetOnMap* GetMapObject() const;
				IPickable* GetPickable() const;
				bool CanPlacePickable() const;
			private:
				virtual bool isAbleToStayOn_Impl() const override;
				virtual void reset_Impl() override;
			private:
				class StaySpaceTileInfoImpl;
				TUniquePtr<StaySpaceTileInfoImpl> m_pImpl;
		};
	}
}
