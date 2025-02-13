// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithPickableLootHandle.h"
#include "DropRatioMode.h"
#include "SmithPickableDropRateListRow.generated.h"

/**
 * 
 */
USTRUCT()
struct SMITH_API FSmithPickableDropRateListRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString ListOwnerName;
	UPROPERTY(EditAnywhere)
	TArray<FSmithPickableLootHandle> DropLootList;
	UPROPERTY(EditAnywhere)
	EDropRatioMode Mode;
};
