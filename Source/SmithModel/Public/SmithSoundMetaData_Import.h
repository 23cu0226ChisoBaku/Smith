// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "SmithSoundMetaData_Import.generated.h"

USTRUCT(BlueprintType, DisplayName = "Sound Meta Data")
struct SMITHMODEL_API FSmithSoundMetaData_Import : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString SoundName;

	UPROPERTY(EditAnywhere)
  TSoftObjectPtr<USoundBase> Sound;
};
