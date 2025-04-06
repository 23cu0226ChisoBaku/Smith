// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "IMapModelGateway.h"

#include "MapModelMapper.generated.h"

class USmithMapModelDefinition;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Const)
class SMITHDATABASE_API UMapModelMapper : public UPrimaryDataAsset , public IMapModelGateway
{
	GENERATED_BODY()

public:

	int32 ReadAll(TArray<FMapModelDTO>& ModelDTOs) const override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "MapModel|Assets")
	TArray<TObjectPtr<USmithMapModelDefinition>> ModelAssets;

	
	
};
