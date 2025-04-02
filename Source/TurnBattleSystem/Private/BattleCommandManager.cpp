// Fill out your copyright notice in the Description page of Project Settings.
/*

BattleCommandManager.cpp

Author : MAI ZHICONG

Description : バトルコマンドを管理するクラス

Update History: 2024/12/16 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "BattleCommandManager.h"

#include "IBattleCommand.h"
#include "ITurnManageable.h"
#include "SmithBattleSubsystem.h"
#include "IEventExecutor.h"
#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCommandManager)

UBattleCommandManager::UBattleCommandManager(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_requestCmdWaitList{}
  , m_commandLists{}
  , m_eventExecutor(nullptr)
  , m_bIsExecutingCommand(false)
  , m_bCanRegister(false)
  , m_bIsReset(false)
{}

void UBattleCommandManager::BeginDestroy()
{
  Super::BeginDestroy();

  OnStartExecuteEvent.Clear();
  OnEndExecuteEvent.Clear();

  Reset();
}

///
/// @brief  イベント処理クラスを登録
///
void UBattleCommandManager::AssignEventExecutor(IEventExecutor* eventExecutor)
{
  m_eventExecutor = eventExecutor;
}

///
///	@brief  コマンド受付リスト登録
///
void UBattleCommandManager::RegisterWaitList(const TArray<TWeakInterfacePtr<ITurnManageable>>& waitList)
{
  for (const auto& waitObj : waitList)
  {
    if (!waitObj.IsValid())
    {
      continue;
    }

    m_requestCmdWaitList.Add(waitObj);
  }
}

///
///	@brief  コマンドを登録する
///
void UBattleCommandManager::RegisterCommand(ITurnManageable* requester, TSharedPtr<IBattleCommand>&& command)
{
  check(requester != nullptr);
  // コマンド実行中に登録禁止
  if (m_bIsExecutingCommand || !m_bCanRegister)
  {
    MDebug::LogError("Cant register during command executing");
    return;
  }

  // 受付リストにあれば受け付ける
  if (m_requestCmdWaitList.Contains(requester))
  {
    requester->SetCommandSendable(false);
    
    m_commandLists.Emplace(::MoveTemp(command));
    m_requestCmdWaitList.Remove(requester);
  }
  else
  {
    MDebug::LogError("Cant Register --- not authorized");
    return;
  }

  // 受付がなくなったらコマンドを実行
  if (m_requestCmdWaitList.Num() == 0)
  {
    m_bIsExecutingCommand = true;
    if (OnStartExecuteEvent.IsBound())
    {
      OnStartExecuteEvent.Broadcast();
      for (auto& registeredCmd : m_commandLists)
      {
        if (registeredCmd.IsValid())
        {
          registeredCmd->Start();
        }
      }
    }
  }
}


///
/// @brief            貯めたコマンドを実行
///
void UBattleCommandManager::ExecuteCommands(float deltaTime)
{
  if (!m_bIsExecutingCommand)
  {
    return;
  }

  // 貯めたコマンドを実行
  for(int32 i = 0; i < m_commandLists.Num(); ++i)
  {
    const auto& command = m_commandLists[i];
    if (command != nullptr)
    {
      command->Execute(deltaTime);
      // 実行終了したらリストから外す
      if (command->IsFinish())
      {
        command->End();
        m_commandLists.RemoveAt(i);

        continue;
      }
    }
  }

  // コマンドがなくなったら受付を再開
  if (m_commandLists.Num() == 0)
  {
    m_bIsExecutingCommand = false;

    // このターンに発生するマップイベントを実行
    if (m_eventExecutor.IsValid() && m_eventExecutor->IsEventInStock())
    { 
      m_eventExecutor->ExecuteEvent();
    }

    if (!m_bIsReset && OnEndExecuteEvent.IsBound())
    {
      OnEndExecuteEvent.Broadcast();
    }
  }
}

///
///	@brief            コマンドマネージャー初期化
///
void UBattleCommandManager::Initialize()
{
  m_bCanRegister = true;
  m_bIsReset = false;
}

///
///	@brief            コマンドマネージャーリセット
///
void UBattleCommandManager::Reset()
{
  m_bIsReset = true;
  m_bIsExecutingCommand = false;
  m_bCanRegister = false;
  m_requestCmdWaitList.Reset();
  m_commandLists.Reset();
}

///
/// @brief  ターン管理オブジェクトを有効状態に調整する
///
void UBattleCommandManager::ForceWaitListValidation()
{
  // 受付リストが空になったらコマンド処理を開始する
  if (m_requestCmdWaitList.Num() == 0)
  {
    m_bIsExecutingCommand = true;
    if (OnStartExecuteEvent.IsBound())
    {
      OnStartExecuteEvent.Broadcast();
      for (auto& registeredCmd : m_commandLists)
      {
        if (registeredCmd.IsValid())
        {
          registeredCmd->Start();
        }
      }
    }
  }

  // 無効のリストオブジェクトを削除
  int32 idx = 0;
  while (idx < m_requestCmdWaitList.Num())
  {
    if (!m_requestCmdWaitList[idx].IsValid())
    {
      m_requestCmdWaitList.RemoveAt(idx);
      continue;
    }

    ++idx;
  }
}
