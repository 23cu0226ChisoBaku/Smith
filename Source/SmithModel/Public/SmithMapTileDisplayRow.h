// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "TileType.h"

#include "SmithMapTileDisplayRow.generated.h"

/**
 * 
 */
USTRUCT()
struct SMITHMODEL_API FSmithMapTileDisplayRow : public FTableRowBase
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere)
  ETileType ObjectType;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UTexture2D> TileDisplayTexture;
};
