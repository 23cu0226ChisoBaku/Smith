// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICanMakeAttack.generated.h"

class IAttackable;
struct AttackHandle;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanMakeAttack : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API ICanMakeAttack
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Attack() = 0;

	virtual void SetAttackTarget(IAttackable*) = 0;
	virtual void SetAttackHandle(AttackHandle&&) = 0;
};
