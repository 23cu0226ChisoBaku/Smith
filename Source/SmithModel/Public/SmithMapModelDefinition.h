// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "SmithMapModelDefinition.generated.h"

UENUM()
enum class EMapModelType : uint8
{
  Player UMETA(DisplayName = "Player"),
  Enemy UMETA(DisplayName = "Enemy"),
  TypeCount UMETA(Hidden),
};	

USTRUCT()
struct FSmithMapModelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
	uint8 MapSizeX;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
	uint8 MapSizeY;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
	EMapModelType MapModelType;

};

UCLASS(BlueprintType, Blueprintable, Const, meta = (DisplayName = "Smith Map Object Definition Asset", ShortTooltip = "Data asset to define map object data"))
class SMITHMODEL_API USmithMapModelDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData")
	TSubclassOf<class AActor> MapObjectActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|MapData", meta = (DisplayName = "DataInstance"))
	FSmithMapModelData MapObjectDataInstance;
	
};
