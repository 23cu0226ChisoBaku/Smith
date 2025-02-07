// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IEnhanceSystem.generated.h"

class IEnhanceable;
class IParamAbsorbable;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnhanceSystem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IEnhanceSystem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
// 引数はあとでWeponとItemのポインタに変える
virtual void Enhance(IEnhanceable*,IParamAbsorbable*) = 0;

};
