// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ITurnManageableWrapper.h"
#include "TurnPriority.h"
#include "SmithBattleSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTurnFinishEvent);

class IBattleCommand;
class UBattleCommandManager;

UCLASS()
class TURNBATTLESYSTEM_API USmithBattleSubsystem final : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	bool ShouldCreateSubsystem(UObject* Outer) const override final;

/** Implement this for initialization of instances of the system */
	void Initialize(FSubsystemCollectionBase& Collection) override final;

	/** Implement this for deinitialization of instances of the system */
	void Deinitialize() override final;

	// TODO Change name
	/// @brief ITurnManageableを継承したActorを登録
	/// 新しいマップが読み込まれたら一回呼ばれる
	void StartBattle();
	
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
public:
	void registerCommand(ITurnManageable*, TSharedPtr<IBattleCommand>);
	// TODO
	void SubscribeOnTurnStartEvent(TDelegate<void()>&);
	void SubscribeOnTurnFinishEvent(TDelegate<void()>&);
private:
	void registerNextTurnObjs();
	void emptyContainers();

private:
	void startExecute();
	void endExecute();

private:
	UPROPERTY()
	TObjectPtr<UBattleCommandManager> m_battleCmdMgr;
	UPROPERTY()
	TMap<ETurnPriority, FITurnManageableWrapper> m_priorityManageableLists;

private:
	FDelegateHandle m_startDelegateHandle;
	FDelegateHandle m_endDelegateHandle;
	ETurnPriority m_curtTurn;
	uint8 m_bCanExecuteCmd : 1;
};
