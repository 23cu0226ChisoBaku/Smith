// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISendableLog.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USendableLog : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API ISendableLog
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual	FString GetName() = 0;
	virtual FColor GetColor() = 0;
};
