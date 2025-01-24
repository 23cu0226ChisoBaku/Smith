// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IOutPutLog.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOutPutLog : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ISendableLog;

class SMITH_API IOutPutLog
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
 virtual	void OutPutAttackLog(ISendableLog* attacker,ISendableLog* defender) = 0;
 virtual	void OutPutUpgradeLog(ISendableLog* owner,ISendableLog* weapon) = 0;
 virtual	void OutPutDamageLog(ISendableLog* defender,int32 damage) = 0;
 virtual	void OutPutKnockDownLog(ISendableLog* downer) = 0;
 virtual	void OutPutGetItemLog(ISendableLog* item,ISendableLog* rider) = 0;
};
