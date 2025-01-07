// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICommandMediator.generated.h"

class IMoveable;

class ICanMakeAttack;
class IAttackable;

class ISkillable;

struct AttackHandle;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommandMediator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITHGOD_API ICommandMediator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SendMoveCommand(AActor*, IMoveable*) = 0;
	virtual void SendAttackCommand(AActor*, ICanMakeAttack*, IAttackable*, AttackHandle&&) = 0;
	virtual void SendSkillCommand(AActor*,ISkillable*) = 0;
};
