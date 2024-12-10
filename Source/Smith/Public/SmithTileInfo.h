// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Map Tile Info

Author : MAI ZHICONG

Description : Tile Information(Use by MapManager)

Update History: 2024/11/25 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_TILE_INFO
#define SMITH_TILE_INFO

#include "CoreMinimal.h"

class FSmithMapObj;

struct SMITH_API FSmithTileInfo
{

	public:
		enum class ETileType : uint8
		{
			Floor,
			Wall,

		};

	public:
		FIntVector2 m_coord;
		ETileType m_tileType;
		FSmithMapObj* m_mapObj;

};

#endif
