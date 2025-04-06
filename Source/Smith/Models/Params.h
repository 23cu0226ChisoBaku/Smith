// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Params.generated.h"

USTRUCT(BlueprintType)
struct SMITH_API FParams
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

	FParams& operator+=(const FParams& Rhs);
};

FParams SMITH_API operator+(const FParams& Lhs, const FParams& Rhs);

