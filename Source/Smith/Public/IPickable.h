// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IPickable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickable : public UInterface
{
	GENERATED_BODY()
};

class SMITH_API IPickable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FString GetPickType() const = 0;
};
