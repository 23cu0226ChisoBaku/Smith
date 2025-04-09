// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "ICharacterEQDefModelGateway.h"

#include "CharacterDefaultEquipmentsMapper.generated.h"

class USmithEquipmentDefinition;
class AActor;

UCLASS(BlueprintType, Blueprintable, Const)
class SMITHDATABASE_API UCharacterDefaultEquipmentsMapper : public UPrimaryDataAsset
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "Character|EquipmentDefinition", meta = (DisplayName = "User Class"))
	TSubclassOf<AActor> EquipmentUserClass;

	UPROPERTY(EditDefaultsOnly, Category = "Character|EquipmentDefinition", meta = (DisplayName = "Equipment Definition"))
	TSet<TObjectPtr<USmithEquipmentDefinition>> EquipmentDefs;

	
};
