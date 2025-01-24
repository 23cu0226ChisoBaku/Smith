// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICanUseEnhanceSystem.generated.h"

class IEnhanceSystem;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanUseEnhanceSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API ICanUseEnhanceSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetEnhanceSystem(IEnhanceSystem*) = 0;
};
