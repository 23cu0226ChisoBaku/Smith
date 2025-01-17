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
	UPROPERTY(EditAnywhere, Category = "Params|Edit")
	int32 HP;
	UPROPERTY(EditAnywhere, Category = "Params|Edit")
	int32 ATK;
	UPROPERTY(EditAnywhere, Category = "Params|Edit")
	int32 DEF;
	UPROPERTY(EditAnywhere, Category = "Params|Edit")
	int32 CRT;
};

FParams operator+(const FParams& lhs, const FParams& rhs);

