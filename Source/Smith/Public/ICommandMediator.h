// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICommandMediator.generated.h"

class IMoveable;
class ICanMakeAttack;
class IAttackable;
struct AttackHandle;
struct FAttackHandle;
enum class EDirection : uint8;

namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
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
	virtual bool SendMoveCommand(AActor*, IMoveable*, EDirection, uint8 moveDistance) = 0;
	virtual bool SendAttackCommand(AActor*, ICanMakeAttack*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, AttackHandle&&, bool bAttackEvenNoTarget = true) = 0;
	virtual bool SendIdleCommand(AActor*) = 0;

	virtual bool SendAttackCommand(AActor*, ICanMakeAttack*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&, bool bAttackEvenNoTarget = true);
};
