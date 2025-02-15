// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TileType.h"

#include "SmithMapData_Import.generated.h"
/**
 * 
 */
USTRUCT(Blueprintable, DisplayName = "MapData")
struct SMITHMODEL_API FSmithMapData_Import : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Row;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Column;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETileType> TileTypes;
};
