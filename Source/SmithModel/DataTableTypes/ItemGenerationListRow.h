// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "MapDeployRule.h"

#include "ItemGenerationListRow.generated.h"

USTRUCT()
struct SMITHMODEL_API FItemGenerationListRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EMapDeployRule DeployRule;

	UPROPERTY(EditAnywhere, meta = (MustImplement="Pickable"))
	TSubclassOf<UObject> Item;
	
	UPROPERTY(EditAnywhere)
	uint32 SpawnCount;
};
