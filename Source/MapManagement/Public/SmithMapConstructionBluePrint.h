// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileType.h"
#include "SmithMapConstructionBluePrint.generated.h"


USTRUCT(BlueprintType)
struct MAPMANAGEMENT_API FSmithMapConstructionBluePrint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TileSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OriginCoordinate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETileType, FString> TileBuildingMaterialPaths;

};
