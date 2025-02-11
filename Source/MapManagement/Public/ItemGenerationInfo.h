// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemGenerationInfo.generated.h"


UENUM()
enum class EMapDeployRule
{
	Random UMETA(DisplayName="Random Deployment"),
	Corner UMETA(DisplayName="Corner Deployment"),
	Sides_With_Corner UMETA(DisplayName="Side Deployment"),
	Sides_Without_Corner UMETA(DisplayName="Side without corner Deployment"),
};
/**
 * 
 */
USTRUCT(BlueprintType)
struct MAPMANAGEMENT_API FItemGenerationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EMapDeployRule DeployRule;
	UPROPERTY(EditAnywhere, meta=(MustImplement="Pickable"))
	TSubclassOf<UObject> Item;

};
