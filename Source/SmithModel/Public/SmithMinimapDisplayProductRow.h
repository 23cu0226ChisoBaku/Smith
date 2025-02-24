// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MinimapDisplayType.h"
#include "SmithMinimapDisplayProductRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITHMODEL_API FSmithMinimapDisplayProductRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EMinimapDisplayType Type;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> DisplayTexture;

};
