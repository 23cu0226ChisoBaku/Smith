// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithPickableLootHandle.generated.h"

USTRUCT()
struct SMITH_API FSmithPickableLootHandle
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(MustImplement="Pickable"))
	TSubclassOf<UObject> LootSubclass;
	UPROPERTY(EditAnywhere)
	float Ratio;
};
