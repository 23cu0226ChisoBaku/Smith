// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IMinimapDisplayable.generated.h"

class UTexture2D;

UINTERFACE(MinimalAPI, Blueprintable)
class UMinimapDisplayable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IMinimapDisplayable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UTexture2D* GetMinimapDisplayTexture();

};
