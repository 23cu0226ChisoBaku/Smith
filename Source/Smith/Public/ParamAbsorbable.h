// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SmithActors/Weapon/Params.h"
#include "ParamAbsorbable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UParamAbsorbable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IParamAbsorbable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual Params GetParam() = 0;
};
