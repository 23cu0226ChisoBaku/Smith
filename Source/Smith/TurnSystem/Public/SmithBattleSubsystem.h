// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ITurnManageableWrapper.h"
#include "TurnPriority.h"
#include "Debug.h"
#include "SmithBattleSubsystem.generated.h"

class IBattleCommand;
class UBattleCommandManager;

UCLASS()
class SMITH_API USmithBattleSubsystem final : public UTickableWorldSubsystem
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
	void RegisterTurnObj();
	
	// Test Func
	void YAYA()
	{
		FString str{};

		for (const auto& pair : m_priorityLists)
		{
			for (const auto& actor : pair.Value.Elements)
			{
				str.Reset();
				// const UEnum* EnumPtr = FindObject<UEnum>(nullptr, TEXT("/Script/Smith.ETurnPriority"));
				// if (EnumPtr != nullptr)
				// {
				// 	str.Append(EnumPtr->GetNameByValue(StaticCast<int64>(pair.Key)).ToString());
				// }

				str.Append(Cast<AActor>(actor.Get())->GetName());
				UE::MLibrary::Debug::LogWarning(str);
			}
		}
	}

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



private:
	void registerCommand(ITurnManageable*, TSharedPtr<IBattleCommand>);
	void registerNextTurnObjs();
	void emptyContainers();

private:
	void startExecute();
	void endExecute();

private:
	UPROPERTY()
	TMap<ETurnPriority, FITurnManageableWrapper> m_priorityLists;
	UPROPERTY()
	TObjectPtr<UBattleCommandManager> m_battleCmdMgr;

private:
	ETurnPriority m_curtTurn;
	uint8 m_bCanExecuteCmd : 1;
};
