// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "BattleCommandManager.generated.h"

class IBattleCommand;
class ITurnManageable;
class UBattleCommandManager;

DECLARE_EVENT(UBattleCommandManager, FStartEndEvent)


UCLASS()
class TURNBATTLESYSTEM_API UBattleCommandManager final: public UObject
{
	GENERATED_BODY()

public:
	UBattleCommandManager(const FObjectInitializer& ObjectInitializer);
	
	void BeginDestroy() override final;

// start of UBattleCommandManager Interface
#pragma region UBattleCommandManager Interface
public:
	void RegisterWaitList(const TArray<TWeakInterfacePtr<ITurnManageable>>&);
	void RegisterCommand(ITurnManageable*, TSharedPtr<IBattleCommand>&&);
	void ExecuteCommands(float deltaTime);
#pragma endregion
// end of UBattleCommandManager Interface

// start of Test Code
#pragma region Test Code
public:
	void Test();
#pragma endregion
// end of Test Code

public:
	FStartEndEvent OnStartExecuteEvent;
	FStartEndEvent OnEndExecuteEvent;
private:
	TArray<ITurnManageable*> m_requestCmdWaitList;
	TArray<TSharedPtr<IBattleCommand>> m_commandLists;
	uint8 m_bIsExecutingCommand : 1;

};
