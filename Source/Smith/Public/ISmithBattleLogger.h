// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISmithBattleLogger.generated.h"

enum class EBattleLogType : uint8
{
	Player = 0,
	Enemy = 1,
	Item = 2,
	None = 3,
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USmithBattleLogger : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API ISmithBattleLogger
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FString GetName_Log() const;
	virtual EBattleLogType GetType_Log() const;

};
