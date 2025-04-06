// Fill out your copyright notice in the Description page of Project Settings.


#include "MapModelMapper.h"

#include "SmithMapModelDefinition.h"

int32 UMapModelMapper::ReadAll(TArray<FMapModelDTO>& ModelDTOs) const
{
  ModelDTOs.Reset();

  for (USmithMapModelDefinition* asset : ModelAssets)
  {
    if (asset == nullptr)
    {
      continue;
    }

    FMapModelDTO dto = 
    {
      .ActorClass = asset->MapObjectActorClass,
      .SizeX = asset->MapObjectDataInstance.MapSizeX,
      .SizeY = asset->MapObjectDataInstance.MapSizeY,
      .Type = asset->MapObjectDataInstance.MapModelType
    };

    ModelDTOs.AddUnique(dto);
  }

  return ModelDTOs.Num();
}

