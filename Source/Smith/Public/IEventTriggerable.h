// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEventTriggerable.generated.h"

class USmithNextLevelEvent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEventTriggerable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IEventTriggerable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnTriggerEvent(USmithNextLevelEvent*) = 0;
};