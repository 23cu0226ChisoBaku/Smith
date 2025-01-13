// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICommandMediator.generated.h"

class IMoveable;

class ICanMakeAttack;
class IAttackable;
struct AttackHandle;

namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
		enum class EMoveDirection : uint8;
	}
}

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommandMediator : public UInterface
{
	GENERATED_BODY()
};

class SMITH_API ICommandMediator
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SendMoveCommand(AActor*, IMoveable*, UE::Smith::Battle::EMoveDirection, uint8 moveDistance) = 0;
	virtual void SendAttackCommand(AActor*, ICanMakeAttack*, UE::Smith::Battle::EMoveDirection, const UE::Smith::Battle::FSmithCommandFormat&, AttackHandle&&) = 0;
};
