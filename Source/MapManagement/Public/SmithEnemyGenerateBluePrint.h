// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithEnemyGenerateBluePrint.h

Author : MAI ZHICONG

Description : マップに敵を生成する設計図

Update History: 2024/01/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"
#include "SmithEnemyGenerateBluePrint.generated.h"

/**
 *	敵を生成する設計図
 *  ※Smith鍛冶師専用設計図
 */
USTRUCT(BlueprintType)
struct MAPMANAGEMENT_API FSmithEnemyGenerateBluePrint
{
	GENERATED_BODY()

	/** 敵のBPパス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyBPPath;
	/** 部屋ごとに敵を生成する数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 InitGenerateCountPerRoom;
	/** 未使用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 GenerateNewWhenLessThan_Test;
	/** 未使用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 GenerateNew_TurnInterval;

};
