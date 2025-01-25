// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithInventoryInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITH_API FSmithInventoryInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InventoryInfo, meta = (AllowAbstract = "true"))
	TSubclassOf<UObject> ObjectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InventoryInfo)
	int32 InventoryCapacity;
	UPROPERTY()
	TArray<TObjectPtr<UObject>> ObjectContainer;

};
