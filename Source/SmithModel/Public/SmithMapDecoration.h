// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MapDeployRule.h"

#include "SmithMapDecoration.generated.h"

USTRUCT()
struct SMITHMODEL_API FSmithMapDecoration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> MapDecoration;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
	int32 DecorationCount;
	
	UPROPERTY(EditAnywhere)
	EMapDeployRule DeployRule;

};
