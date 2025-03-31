// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "BattleResult.h"

#include "IAttackable.generated.h"

struct FBattleDefenseParamHandle;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackable : public UInterface
{
	GENERATED_BODY()
};

///
/// @brief 攻撃できる
///
class SMITH_API IAttackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnAttack(const FBattleResult& Result) = 0;
	virtual bool IsDefeated() const = 0;
	virtual void OnDefeated() = 0;
	virtual FBattleDefenseParamHandle GetDefenseParam() const;
};
