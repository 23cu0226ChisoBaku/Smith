// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Direction.h"
#include "SmithSkillCenterSpotParameter.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITH_API FSmithSkillCenterSpotParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	EDirection Direction;
	UPROPERTY(EditAnywhere)
	uint8 OffsetToLeft;
	UPROPERTY(EditAnywhere)
	uint8 OffsetToTop;
};
