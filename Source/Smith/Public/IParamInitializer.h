// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SmithEnemyTraits.h"
#include "../Weapon/Params.h"
#include "IParamInitializer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UParamInitializer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API IParamInitializer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FParams Initialize(SmallGolem, int32 currentLevel) = 0;
	virtual FParams Initialize(Dragon, int32 currentLevel) = 0;
	virtual FParams Initialize(HerbGolem, int32 currentLevel) = 0;
};
