// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MAP_FORMAT
#define SMITH_MAP_FORMAT

#include "CoreMinimal.h"

#include "DataStructure/Dimension2Array.h"
#include "TileType.h"

namespace UE::Smith
{
	namespace Map
	{
		class SMITHMODEL_API FSmithMapFormat
		{
			public:
				FSmithMapFormat();
				FSmithMapFormat(const ETileType* srcData, size_t dataCnt, uint64 row, uint64 column);
				~FSmithMapFormat();

				FSmithMapFormat(const FSmithMapFormat&);
				FSmithMapFormat& operator=(const FSmithMapFormat&);

				FSmithMapFormat(FSmithMapFormat&&) noexcept;
				FSmithMapFormat& operator=(FSmithMapFormat&&) noexcept;

			public:
				void SetupMapFormat(const ETileType* srcData, size_t dataCnt, uint64 row, uint64 column);
				UE::MLibrary::MDataStructure::TDimension2Array<ETileType> GetFormat() const;
				uint64 GetRow() const;
				uint64 GetColumn() const;

			private:
				UE::MLibrary::MDataStructure::TDimension2Array<ETileType> m_format;
		};
	}
}

#endif
