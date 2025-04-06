// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataAsset.h"

#include "IBattleLogModelGateway.h"

#include "BattleLogModelMapper.generated.h"

class USmithBattleLogModelDefinition;

UCLASS(BlueprintType, Blueprintable, Const)
class SMITHDATABASE_API UBattleLogModelMapper : public UPrimaryDataAsset , public IBattleLogModelGateway
{
	GENERATED_BODY()
	
public:

	int32 ReadAll(TArray<FBattleLogModelDTO>& ModelDTOs) const override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "BattleLogModel|Assets")
	TArray<TObjectPtr<USmithBattleLogModelDefinition>> ModelAssets;
};
