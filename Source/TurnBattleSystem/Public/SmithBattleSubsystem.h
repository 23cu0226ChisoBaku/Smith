// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithBattleSubsystem.h

Author : MAI ZHICONG

Description : ターン制バトルサブシステム

Update History: 2024/12/13 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "UObject/WeakInterfacePtr.h"
#include "TurnPriority.h"

#include "SmithBattleSubsystem.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class IBattleCommand;
class UBattleCommandManager;
class IEventExecutor;
class ITurnManageable;
class FSmithBattleCommand;

///
/// @brief バトルサブシステム
/// UTickableWorldSubsystemから派生
///
UCLASS()
class TURNBATTLESYSTEM_API USmithBattleSubsystem final : public UTickableWorldSubsystem
{
  GENERATED_BODY()

public:
  //---Begin of USubsystem Interface
  bool ShouldCreateSubsystem(UObject* Outer) const override final;
  void Initialize(FSubsystemCollectionBase& Collection) override final;
  void Deinitialize() override final;
  //---End of USubsystem Interface

  ///
  /// @brief バトルシステムを初期化
  /// 
  void InitializeBattle();
  ///
  /// @brief バトルシステムをリセットする
  ///
  void ResetBattle();
  
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
  ///
  /// @brief                イベント処理クラスを登録
  /// @param IEventExecutor イベント処理インターフェース
  ///  
  void AssignEventExecutor(IEventExecutor*);
  ///
  ///	@brief                              コマンドを登録する
  /// @param ITurnManageable              ターン管理オブジェクト
  /// @param TSharedPtr<IBattleCommand>   コマンド
  ///
  void RegisterCommand(ITurnManageable*, TSharedPtr<IBattleCommand>);

private:
  /// 
  /// @brief 次のターンのオブジェクトリストを登録
  ///
  void registerNextTurnObjs();
  /// 
  /// @brief コマンド実行開始
  ///
  void startExecute();
  /// 
  /// @brief コマンド実行終了
  ///
  void endExecute();

private:
  // コマンドマネージャー
  UPROPERTY()
  TObjectPtr<UBattleCommandManager> m_battleCmdMgr;
  // ターンマネジメントオブジェクトリスト

  TMap<ETurnPriority, TArray<TWeakInterfacePtr<ITurnManageable>>> m_priorityManageableLists;

private:

  FDelegateHandle m_startDelegateHandle;
  FDelegateHandle m_endDelegateHandle;
  ETurnPriority m_curtTurn;
  uint8 m_bCanExecuteCmd : 1;
  uint8 m_bIsInitialized : 1 = false;
};
