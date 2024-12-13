// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ITurnManageableWrapper.generated.h"

class ITurnManageable;
/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITH_API FITurnManageableWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<ITurnManageable*> Elements;
};