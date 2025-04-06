// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "MapModelType.h"

#include "IMapModelGateway.generated.h"

struct SMITHMODEL_API FMapModelDTO
{
  TSubclassOf<AActor> ActorClass;
  uint8 SizeX = 0u;
  uint8 SizeY = 0u;
  EMapModelType Type = EMapModelType::TypeCount;
};

bool SMITHMODEL_API operator==(const FMapModelDTO& Lhs, const FMapModelDTO Rhs);

UINTERFACE(MinimalAPI)
class UMapModelGateway : public UInterface
{
  GENERATED_BODY()
};

class SMITHMODEL_API IMapModelGateway
{
  GENERATED_BODY()

public:

  virtual int32 ReadAll(TArray<FMapModelDTO>& ModelDTOs) const = 0;
};
