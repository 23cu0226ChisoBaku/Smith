// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnManager_Test.generated.h"

class ITurnManageable;
enum class ETurnPriority : uint8;

USTRUCT(FITurnManageableWrapper)
struct SMITH_API FITurnManageableWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<ITurnManageable*> Elements;
};

UCLASS()
class SMITH_API UTurnManager_Test : public UObject
{
	GENERATED_BODY()

public:
	UTurnManager_Test();
	~UTurnManager_Test();

public:
	void RegisterManageable(ETurnPriority, ITurnManageable*);

private:
	void onExecuteCommand(ITurnManageable*);

private:
	UPROPERTY()
	TMap<ETurnPriority,FITurnManageableWrapper> m_manageableContainer;
};
