// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SmithSoundMetaData_Import.generated.h"

USTRUCT(Blueprintable, DisplayName = "Sound Meta Data")
struct SMITHMODEL_API FSmithSoundMetaData_Import : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SoundName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TSoftObjectPtr<USoundBase> Sound;
};
