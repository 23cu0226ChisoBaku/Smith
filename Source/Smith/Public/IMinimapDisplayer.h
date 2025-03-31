// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "IMinimapDisplayer.generated.h"

struct FMapCoord;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMinimapDisplayer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IMinimapDisplayer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AssignMapData(const TArray<UObject*>& mapItems, uint8 row, uint8 column) = 0;
	virtual void ReplaceTiles(const TArray<UObject*>&, const FMapCoord& originCoord, uint8 sizeX, uint8 sizeY) = 0;
	virtual void UpdateMinimap(UObject*, const FMapCoord&, uint8 sizeX, uint8 sizeY) = 0;
	virtual void SetVisibility(bool bIsVisible) = 0;
	virtual void ResetMap() = 0;
};
