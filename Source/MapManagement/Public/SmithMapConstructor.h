// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MAP_CONSTRUCTOR
#define SMITH_MAP_CONSTRUCTOR

#include "CoreMinimal.h"

struct FSmithMapConstructionBluePrint;
enum class ETileType : uint8;

namespace UE::Smith
{
	namespace Map
	{
		class FSmithRect;

		class MAPMANAGEMENT_API FSmithMapConstructor
		{
			public:
				FSmithMapConstructor();
				~FSmithMapConstructor();

			public:
				void ConstructMap(UWorld*, const FSmithRect&, const FSmithMapConstructionBluePrint&);
		};
	}
}

#endif