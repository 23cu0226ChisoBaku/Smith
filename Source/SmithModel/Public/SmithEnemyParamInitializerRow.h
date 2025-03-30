// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"

#include "../SmithActors/Weapon/Params.h"

#include "SmithEnemyParamInitializerRow.generated.h"

USTRUCT()
struct SMITHMODEL_API FSmithEnemyParamInitializerRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString EnemyName;
	
	UPROPERTY(EditAnywhere)
	FParams DefaultParam;
};
