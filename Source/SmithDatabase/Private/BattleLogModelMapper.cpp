// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleLogModelMapper.h"

#include "SmithBattleLogModelDefinition.h"

int32 UBattleLogModelMapper::ReadAll(TArray<FBattleLogModelDTO>& ModelDTOs) const
{
  ModelDTOs.Reset();

  for (USmithBattleLogModelDefinition* asset : ModelAssets)
  {
    if (asset == nullptr)
    {
      continue;
    }

    FBattleLogModelDTO dto = 
    {
      .LogModelClass = asset->LogClass,
      .Name = asset->LogData.Name,
      .LogType = asset->LogData.Type
    };

    ModelDTOs.AddUnique(dto);
  }

  return ModelDTOs.Num();
}