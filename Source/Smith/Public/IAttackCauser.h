// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IAttackCauser.generated.h"

UINTERFACE(MinimalAPI)
class UAttackCauser : public UInterface
{
	GENERATED_BODY()
};

class SMITH_API IAttackCauser
{
	GENERATED_BODY()

public:
	///
	/// @brief 					攻撃を通ったら呼び出されるコールバック
	///
	virtual void OnAttackExecuted();
};
