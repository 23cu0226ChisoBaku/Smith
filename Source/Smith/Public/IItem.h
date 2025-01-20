// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IItem.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
struct FParams;
class SMITH_API IItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FParams GetParam() = 0;
	virtual FString GetItemName() = 0;
};
