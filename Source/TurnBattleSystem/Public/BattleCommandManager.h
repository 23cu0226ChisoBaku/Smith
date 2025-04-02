// Fill out your copyright notice in the Description page of Project Settings.
/*

BattleCommandManager.h

Author : MAI ZHICONG

Description : バトルコマンドを管理するクラス

Update History: 2024/12/16 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "UObject/NoExportTypes.h"
#include "UObject/WeakInterfacePtr.h"

#include "BattleCommandManager.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class IBattleCommand;
class ITurnManageable;
class UBattleCommandManager;
class IEventExecutor;

///
/// @brief バトルコマンドの実行、管理などを行うクラス
///
UCLASS()
class UBattleCommandManager final: public UObject
{
  GENERATED_BODY()

public:
  UBattleCommandManager(const FObjectInitializer& ObjectInitializer);
  void BeginDestroy() override final;

// start of UBattleCommandManager Interface
#pragma region UBattleCommandManager Interface
public:
  ///
  /// @brief                イベント処理クラスを登録
  /// @param IEventExecutor イベント処理インターフェース
  ///  
  void AssignEventExecutor(IEventExecutor*);
  ///
  ///	@brief                コマンド受付リスト登録						
  /// @param                コマンド受付リスト
  ///
  void RegisterWaitList(const TArray<TWeakInterfacePtr<ITurnManageable>>&);
  ///
  ///	@brief                              コマンドを登録する
  /// @param ITurnManageable              ターン管理オブジェクト
  /// @param TSharedPtr<IBattleCommand>   コマンド
  ///
  void RegisterCommand(ITurnManageable*, TSharedPtr<IBattleCommand>&&);
  ///
  /// @brief            貯めたコマンドを実行
  /// @param deltaTime  デルタタイム
  /// 
  void ExecuteCommands(float deltaTime);
  ///
  ///	@brief            コマンドマネージャー初期化
  ///
  void Initialize();
  ///
  ///	@brief            コマンドマネージャーリセット
  ///
  void Reset();

  /// TODO もっといい名前にする
  ///
  /// @brief            ターン管理オブジェクトを有効性チェック
  ///
  void ForceWaitListValidation();
#pragma endregion
// end of UBattleCommandManager Interface

public:

  TMulticastDelegate<void()> OnStartExecuteEvent;
  TMulticastDelegate<void()> OnEndExecuteEvent;

private:
  TArray<TWeakInterfacePtr<ITurnManageable>> m_requestCmdWaitList;
  TArray<TSharedPtr<IBattleCommand>> m_commandLists;
  TWeakInterfacePtr<IEventExecutor> m_eventExecutor;
  uint8 m_bIsExecutingCommand : 1;
  uint8 m_bCanRegister : 1;
  uint8 m_bIsReset : 1;

};
