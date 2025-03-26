// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "MapSizeInfoRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITHMODEL_API FMapSizeInfoRow : public FTableRowBase
{
	 GENERATED_BODY()

	 UPROPERTY(EditAnywhere)
	 TSubclassOf<AActor> MapActorClass;

	 UPROPERTY(EditAnywhere)
	 uint8 SizeX;

	 UPROPERTY(EditAnywhere)
	 uint8 SizeY;
};
