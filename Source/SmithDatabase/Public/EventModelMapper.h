// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "IEventModelGateway.h"

#include "EventModelMapper.generated.h"

class USmithEventModelDefinition;

UCLASS(BlueprintType, Blueprintable, Const)
class SMITHDATABASE_API UEventModelMapper : public UPrimaryDataAsset , public IEventModelGateway
{
	GENERATED_BODY()

public:

	int32 ReadAll(TArray<FEventModelDTO>& ModelDTOs) const override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "EventModel|Assets")
	TArray<TObjectPtr<USmithEventModelDefinition>> ModelAssets;
	
};
