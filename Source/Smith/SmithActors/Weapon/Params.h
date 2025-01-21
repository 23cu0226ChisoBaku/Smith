// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Params.generated.h"

/**
 * 
 */
USTRUCT()
struct SMITH_API FParams
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 HP;
	UPROPERTY(EditAnywhere)
	int32 ATK;
	UPROPERTY(EditAnywhere)
	int32 DEF;
	UPROPERTY(EditAnywhere)
	int32 CRT;
};

FParams operator+(const FParams& lhs, const FParams& rhs);

