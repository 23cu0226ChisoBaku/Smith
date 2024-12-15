// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ITurnManageableWrapper.h"
#include "TurnPriority.h"
#include "SmithBattleSubsystem.generated.h"

class IBattleCommand;
class UBattleCommandManager;

UCLASS()
class SMITH_API USmithBattleSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	bool ShouldCreateSubsystem(UObject* Outer) const override final;

/** Implement this for initialization of instances of the system */
	void Initialize(FSubsystemCollectionBase& Collection) override final;

	/** Implement this for deinitialization of instances of the system */
	void Deinitialize() override final;

	/// @brief ITurnManageableを継承したActorを登録
	/// 新しいマップが読み込まれたら一回呼ばれる
	void RegisterTurnObj();
	
	// Test Func
	void YAYA()
	{
		FString str{};
		if (GEngine != nullptr)
		{
			for (const auto& pair : m_priorityLists)
			{
				for (const auto& actor : pair.Value.Elements)
				{
					str.Reset();
					const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ETurnPriority"), true);
					if (EnumPtr != nullptr)
					{
						str.Append(EnumPtr->GetNameByValue(StaticCast<int64>(pair.Key)).ToString());
					}

					str.Append(Cast<AActor>(actor.Get())->GetName());
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, str);
				}
			}
		}
	}

private:
	void executeCommand();
	void registerCommand(ITurnManageable*, TSharedPtr<IBattleCommand>);
	void registerNextTurnObjs();
	void emptyContainers();

private:
	UPROPERTY()
	TMap<ETurnPriority, FITurnManageableWrapper> m_priorityLists;
	UPROPERTY()
	TArray<ITurnManageable*> m_requestCmdWaitList;
	UPROPERTY()
	TObjectPtr<UBattleCommandManager> m_battleCmdMgr;

private:
	TQueue<TSharedPtr<IBattleCommand>> m_cmdExecuteQueue;
	ETurnPriority m_curtTurn;
};
