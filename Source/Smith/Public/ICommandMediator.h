// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICommandMediator.generated.h"

class IMoveable;
class ICanMakeAttack;
class IAttackable;
class IHealable;
struct AttackHandle;
struct FAttackHandle;
struct FSmithSkillParameter;
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

	virtual bool SendAttackCommand(AActor*, ICanMakeAttack*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&, bool bAttackEvenNoTarget = true) = 0;
	virtual bool SendSkillCommand(AActor*, ICanMakeAttack*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&) = 0;
	virtual bool SendHealCommand(AActor*,IHealable*) = 0;

	// TODO
	virtual int32 GetRangeLocations(TArray<FVector>& outLocations, AActor*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&) const = 0;
	virtual void GetPlayerDirection(EDirection& outDirection, AActor*, uint8 offsetToLeft = 0, uint8 offsetToTop = 0) = 0;
};
