// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIStrategy.h"
#include "SmithAIRegistry.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITH_API FSmithAIRegistry
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRegistry)
	int32 Priority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIRegistry)
	TSubclassOf<USmithAIStrategy> AISubclass;

};
