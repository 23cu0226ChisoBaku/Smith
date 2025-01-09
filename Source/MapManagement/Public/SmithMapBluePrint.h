// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithMapBluePrint.h

Author : MAI ZHICONG

Description : マップを作成するための設計図

Update History: 2024/12/31 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"
#include "TileType.h"
#include "SmithMapBluePrint.generated.h"

USTRUCT(BlueprintType)
struct MAPMANAGEMENT_API FSmithMapBluePrint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SectionRow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SectionColumn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SectionWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SectionHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SectionGap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 RoomMinWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 RoomMaxWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 RoomMinHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 RoomMaxHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 RoomGenerateMinNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 RoomGenerateMaxNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType DefaultSectionTileType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType DefaultRoomTileType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType DefaultCorridorTileType;

};
