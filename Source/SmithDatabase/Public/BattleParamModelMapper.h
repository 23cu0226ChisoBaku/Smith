// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "IBattleParamModelGateway.h"

#include "BattleParamModelMapper.generated.h"

class USmithBattleParamModelDefinition;


UCLASS(Const)
class SMITHDATABASE_API UBattleParamModelMapper : public UPrimaryDataAsset, public IBattleParamModelGateway
{
	GENERATED_BODY()

public:

	int32 ReadAll(TArray<FBattleParamDTO>& ModelDTOs) const override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "ModelAssets")
	TArray<TObjectPtr<USmithBattleParamModelDefinition>> ModelAssets; 
	
};

