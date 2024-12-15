// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "BattleCommandManager.generated.h"

class IBattleCommand;

UCLASS()
class SMITH_API UBattleCommandManager final: public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UBattleCommandManager(const FObjectInitializer& ObjectInitializer);

// start of FTickableObjectBase Interface
#pragma region FTickableObjectBase Interface
	void Tick(float DeltaTime) override final;
	bool IsTickable() const override final;
	TStatId GetStatId() const override final;
#pragma endregion
// end of FTickableObjectBase Interface

// start of FTickableObject Interface
#pragma region FTickableObject Interface
	bool IsTickableWhenPaused() const override final;
	bool IsTickableInEditor() const override final;
	UWorld* GetTickableGameObjectWorld() const override final;
#pragma endregion
// end of FTickableObject Interface

// start of UObject Interface
#pragma region UObject Interface
	class UWorld* GetWorld() const override final;
#pragma endregion
// end of UObject Interface

// start of UBattleCommandManager Interface
#pragma region UBattleCommandManager Interface
public:
	void RegisterCommand(IBattleCommand*);
	void StartCommands();
#pragma endregion
// end of UBattleCommandManager Interface

private:
	void executeCommands();
	void resetCommands();

private:
	TArray<IBattleCommand*> m_commandLists;
	uint8 m_bIsExecutingCommand : 1;
};
