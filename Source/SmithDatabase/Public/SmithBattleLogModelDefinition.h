// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "BattleLogType.h"

#include "SmithBattleLogModelDefinition.generated.h"

/// @brief	バトルログモデルデータ
USTRUCT()
struct FBattleLogModelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Smith|BattleLogData", meta = (DisplayName = "LogName"))
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|BattleLogData", meta = (DisplayName = "LogType"))
	EBattleLogModelType Type;
};






/// @brief	バトルログモデル定義アセット
UCLASS(BlueprintType, Blueprintable, Const)
class USmithBattleLogModelDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/// @brief 具体オブジェクトのUClass（型）
	UPROPERTY(EditDefaultsOnly, Category = "Smith|Log", meta = (DisplayName = "LogEntityClass"))
	TSubclassOf<UObject> LogClass;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|Log")
	FBattleLogModelData LogData;
};
