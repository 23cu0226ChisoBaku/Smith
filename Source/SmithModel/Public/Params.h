// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Params.generated.h"

USTRUCT(BlueprintType)
struct SMITHMODEL_API FParams
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 HP;

	UPROPERTY(EditAnywhere)
	int32 ATK;

	UPROPERTY(EditAnywhere)
	int32 DEF;

	UPROPERTY(EditAnywhere)
	int32 CRT;

	FParams& operator+=(const FParams&);
};

FParams operator+(const FParams& lhs, const FParams& rhs);

