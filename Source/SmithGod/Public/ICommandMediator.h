// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICommandMediator.generated.h"

class IMoveable;

class ICanMakeAttack;
class IAttackable;
struct AttackHandle;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UICommandMediator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITHGOD_API IICommandMediator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SendMoveCommand(IMoveable*) = 0;
	virtual void SendAttackCommand(ICanMakeAttack*, IAttackable*, AttackHandle&&) = 0;
};
