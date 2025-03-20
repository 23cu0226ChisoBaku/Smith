// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithSkillParameter.h"
#include "SmithAIConditionBindHandle.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SMITH_API FSmithAIConditionBindHandle
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	FName ConditionFuncName;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UDataTable> FormatMasterData;
	UPROPERTY(EditAnywhere)
	FSmithSkillParameter SkillParameter;
};
