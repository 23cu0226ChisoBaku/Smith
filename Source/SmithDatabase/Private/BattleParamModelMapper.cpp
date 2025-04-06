// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleParamModelMapper.h"

#include "SmithBattleParamModelDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleParamModelMapper)

int32 UBattleParamModelMapper::ReadAll(TArray<FBattleParamDTO>& ModelDTOs) const
{
  ModelDTOs.Reset();

  for (USmithBattleParamModelDefinition* asset : ModelAssets)
  {
    if ((asset == nullptr) || (asset->ParamClass == nullptr))
    {
      continue;
    }

    FBattleParamDTO dto = 
    {
      .ParamClass = asset->ParamClass,
      .HitPoints = asset->ParamData.DefaultHitPoints,
      .Attack = asset->ParamData.DefaultAttack,
      .Defense = asset->ParamData.DefaultDefense
    };

    ModelDTOs.AddUnique(dto);
  }

  return ModelDTOs.Num();
}