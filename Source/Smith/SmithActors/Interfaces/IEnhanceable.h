// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnhanceable.generated.h"

struct FParams;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnhanceable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IEnhanceable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
virtual FParams GetParam() = 0;
virtual void SetParam(FParams) = 0;
};