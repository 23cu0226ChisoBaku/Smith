// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "SmithBattleLogModelDefinition.generated.h"

UENUM()
enum class EBattleLogModelType : uint8
{
	Player,
	Enemy,
	Item,
	Enhance,
	ModelTypeCount UMETA(Hidden),
};

USTRUCT()
struct FBattleLogModelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Smith|BattleLogData", meta = (DisplayName = "LogName"))
	FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|BattleLogData", meta = (DisplayName = "LogType"))
	EBattleLogModelType Type;
};







UCLASS()
class SMITHMODEL_API USmithBattleLogModelDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Smith|Log", meta = (DisplayName = "LogEntityClass"))
	TSubclassOf<UObject> LogClass;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|Log")
	FBattleLogModelData LogData;
};
