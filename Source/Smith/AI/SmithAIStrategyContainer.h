// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIStrategyContainer.generated.h"

class USmithAIStrategy;

USTRUCT(BlueprintType)
struct SMITH_API FSmithAIStrategyContainer
{
	GENERATED_BODY()

  UPROPERTY(EditAnywhere)
	int32 Priority;
	UPROPERTY(EditAnywhere,Instanced)
	TObjectPtr<USmithAIStrategy> Strategy;
};

bool operator<(const FSmithAIStrategyContainer&, const FSmithAIStrategyContainer&);
