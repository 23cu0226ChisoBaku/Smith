// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "SmithEquipmentDefinition.generated.h"

USTRUCT()
struct FEquipmentModelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Equipment Name"))
	FString Name;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Icon Image"))
	TObjectPtr<UTexture2D> ImageTexture;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Additional Hit Points"))
	int32 ExtraHitPoints;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Additional Attack"))
	int32 ExtraAttack;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Additional Defense"))
	int32 ExtraDefense;
};



UCLASS(Blueprintable, Const, Abstract)
class SMITHDATABASE_API USmithEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:

	USmithEquipmentDefinition();

	UPROPERTY(EditDefaultsOnly, Category = "Smith|Equipment", meta = (DisplayName = "EquipmentType"))
	TSubclassOf<class USmithEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category = "Smith|Equipment", meta = (DisplayName = "EquipmentInformation"))
	FEquipmentModelData Data;
	
};
