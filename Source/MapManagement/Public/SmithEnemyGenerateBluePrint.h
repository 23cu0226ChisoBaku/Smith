// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithEnemyGenerateBluePrint.generated.h"

USTRUCT(BlueprintType)
struct MAPMANAGEMENT_API FSmithEnemyGenerateBluePrint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyBPPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 InitGenerateCountPerRoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 GenerateNewWhenLessThan_Test;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 GenerateNew_TurnInterval;

};
