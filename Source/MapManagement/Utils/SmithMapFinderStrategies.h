// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

struct FMapCoord;

namespace UE::Smith
{
	namespace Map
	{
		class FSmithMap;

		struct MapFinderStrategy_Corner
		{
			void operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const;
		};
		
		struct MapFinderStrategy_Random
		{
			void operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const;
		};
		
		struct MapFinderStrategy_CornerPerRoom
		{
			void operator()(FSmithMap* Map, TArray<FMapCoord>& OutCoords) const;
		};
	}
}
