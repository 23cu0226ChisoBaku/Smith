// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "SmithEquipmentDefinition.generated.h"



UCLASS(Blueprintable, Const, Abstract)
class SMITHDATABASE_API USmithEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:

	USmithEquipmentDefinition();

	UPROPERTY(EditDefaultsOnly, Category = "Smith|Equipment", meta = (DisplayName = "EquipmentType"))
	TSubclassOf<class USmithEquipmentInstance> InstanceType;
	
};
