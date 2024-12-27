// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileType.h"

#include "SmitTileData_Import.generated.h"
/**
 * 
 */
USTRUCT(Blueprintable, DisplayName = "TileData")
struct SMITHMODEL_API FSmitTileData_Import : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETileType TileType;
};
