// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "SmithCharacterModelDefinition.generated.h"

UCLASS(BlueprintType, Blueprintable, Const)
class SMITHDATABASE_API USmithCharacterModelDefinition : public UDataAsset
{
  GENERATED_BODY()
  
public:

  UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Default Parameter Model"))
  TObjectPtr<class USmithBattleParamModelDefinition> ParamModelDefinition;

  UPROPERTY(EditDEfaultsOnly, meta = (DisplayName = "Default Equipment Set"))
  TArray<TObjectPtr<class USmithEquipmentDefinition>> WeaponSetDef;
};
