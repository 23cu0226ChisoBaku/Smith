// Fill out your copyright notice in the Description page of Project Settings.

#include "EventModelMapper.h"

#include "SmithEventModelDefinition.h"

int32 UEventModelMapper::ReadAll(TArray<FEventModelDTO>& ModelDTOs) const
{
  ModelDTOs.Reset();

  for (USmithEventModelDefinition* asset : ModelAssets)
  {
    if (asset == nullptr)
    {
      continue;
    }

    FEventModelDTO dto =
    {
      .EventClass = asset->EventClass,
      .SucceededMsg = asset->EventData.SucceededMessage,
      .FailedMsg = asset->EventData.FailedMessage
    };

    ModelDTOs.AddUnique(dto);
  }

  return ModelDTOs.Num();
}