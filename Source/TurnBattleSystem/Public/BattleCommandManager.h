// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "UObject/NoExportTypes.h"
#include "BattleCommandManager.generated.h"

class IBattleCommand;
class ITurnManageable;
class UBattleCommandManager;
class IEventExecutor;

DECLARE_MULTICAST_DELEGATE(FStartEndEvent)

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
	void AssignEventExecutor(IEventExecutor*);
	void RegisterWaitList(const TArray<TWeakInterfacePtr<ITurnManageable>>&);
	void RegisterCommand(ITurnManageable*, TSharedPtr<IBattleCommand>&&);
	void ExecuteCommands(float deltaTime);
	void Initialize();
	void Reset();
#pragma endregion
// end of UBattleCommandManager Interface

public:
	FStartEndEvent OnStartExecuteEvent;
	FStartEndEvent OnEndExecuteEvent;
private:
	TArray<ITurnManageable*> m_requestCmdWaitList;
	TArray<TSharedPtr<IBattleCommand>> m_commandLists;
	TWeakInterfacePtr<IEventExecutor> m_eventExecutor;
	uint8 m_bIsExecutingCommand : 1;
	uint8 m_bCanRegister : 1;

};
