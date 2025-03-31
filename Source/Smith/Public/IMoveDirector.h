// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IMoveDirector.generated.h"

class USmithMoveDirector;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMoveDirector : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IMoveDirector
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UClass* GetMoveDirectorUClass() const = 0;
	virtual void SetMoveDirector(USmithMoveDirector*) = 0;
	virtual uint8 GetChaseRadius() const = 0;

};
