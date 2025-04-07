// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "SmithBattleParamModelDefinition.generated.h"

USTRUCT(BlueprintType)
struct FParamModelData
{
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, Category = "SmithBattle|Parameter", meta = (DisplayName = "Hit Points", ShortTooltip = "Hit points when level 1(Default)", ClampMin = "1", UIMin = "1"))
  int32 DefaultHitPoints;

  UPROPERTY(EditDefaultsOnly, Category = "SmithBattle|Parameter", meta = (DisplayName = "Attack", ShortTooltip = "Attack when level 1(Default)", ClampMin = "1", UIMin = "1"))
  int32 DefaultAttack;

  UPROPERTY(EditDefaultsOnly, Category = "SmithBattle|Parameter", meta = (DisplayName = "Defense", ShortTooltip = "Defense when level 1(Default)", ClampMin = "1", UIMin = "1"))
  int32 DefaultDefense;

};











UCLASS(BlueprintType, Blueprintable, Const)
class SMITHDATABASE_API USmithBattleParamModelDefinition : public UDataAsset
{
  GENERATED_BODY()
  
public:

  UPROPERTY(EditDefaultsOnly, Category = "SmithBattle|ParamModel", meta = (ShortTooltip = "パラメーター持ち主クラス"))
  TSubclassOf<UObject> ParamClass;

  UPROPERTY(EditDefaultsOnly, Category = "SmithBattle|ParamModel", meta = (ShortTooltip = "レベル１のパラメーターデータ"))
  FParamModelData ParamData;
};
