// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Direction.h"

#include "SmithSkillParameter.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITH_API FSmithSkillParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	uint8 OffsetToLeft;

	UPROPERTY(EditAnywhere)
	uint8 OffsetToTop;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 3))
	uint8 SkillSlot;

	UPROPERTY(EditAnywhere)
	uint8 CooldownAfterUse;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.0, ClampMax = 5.0))
	double MotionValue;

	EDirection ActiveDirection = EDirection::Invalid;
};
