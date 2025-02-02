// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISmithItemWidgetParameterizable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USmithItemWidgetParameterizable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API ISmithItemWidgetParameterizable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UTexture2D* GetIconImage() const = 0;
	virtual FColor GetLabelColor() const = 0;
	virtual FString GetName() const = 0;
	virtual FString GetDescription() const = 0;
};
