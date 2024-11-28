// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Map Coordinate

Author : MAI ZHICONG

Description : Map Coordinate(Use by Map)

Update History: 2024/11/28 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"
#include "MapCoord.generated.h"


USTRUCT(BlueprintType)
struct SMITH_API FMapCoord
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	uint8 x;
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite)
	uint8 y;
};
