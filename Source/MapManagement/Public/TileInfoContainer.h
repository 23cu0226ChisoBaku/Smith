// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileType.h"

namespace UE::Smith
{
	namespace Map
	{
		class MAPMANAGEMENT_API FTileInfoContainer
		{
		public:
			explicit FTileInfoContainer(ETileType);
			virtual ~FTileInfoContainer() = 0 {};

			FTileInfoContainer(const FTileInfoContainer&) noexcept;
			FTileInfoContainer& operator=(const FTileInfoContainer&) noexcept;

			FTileInfoContainer(FTileInfoContainer&&) noexcept;
			FTileInfoContainer& operator=(FTileInfoContainer&&) noexcept;
		public:
			ETileType GetTileType() const;
			bool IsAbleToStayOn() const;
			void Reset();
		private:
			virtual bool isAbleToStayOn_Impl() const;
			virtual void reset_Impl();
		private:
			ETileType m_tileType;

		};
	}
}
