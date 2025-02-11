// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemGenerationInfo.h"
#include "ItemGenerationListRow.generated.h"


/**
 * 
 */
USTRUCT()
struct MAPMANAGEMENT_API FItemGenerationListRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FItemGenerationInfo Information;
	UPROPERTY(EditAnywhere)
	uint32 SpawnCount;
};
