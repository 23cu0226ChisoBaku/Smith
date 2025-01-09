// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "MapPathContainer_Import.generated.h"

USTRUCT(Blueprintable, DisplayName = "MapPathContainer")
struct SMITHMODEL_API FMapPathContainer_Import : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RelativePath;
};
