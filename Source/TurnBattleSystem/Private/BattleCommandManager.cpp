// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCommandManager.h"
#include "IBattleCommand.h"
#include "SmithBattleSubsystem.h"
#include "MLibrary.h"

UBattleCommandManager::UBattleCommandManager(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{}

void UBattleCommandManager::BeginDestroy()
{
  Super::BeginDestroy();

  OnStartExecuteEvent.Clear();
  OnEndExecuteEvent.Clear();

  m_requestCmdWaitList.Empty();
  m_commandLists.Empty();

  m_bIsExecutingCommand = false;

}

void UBattleCommandManager::RegisterWaitList(const TArray<TWeakInterfacePtr<ITurnManageable>>& waitList)
{
  for (auto waitObj : waitList)
  {
    if (!waitObj.IsValid())
    {
      continue;
    }

    m_requestCmdWaitList.Add(waitObj.Get());
  }
}

void UBattleCommandManager::RegisterCommand(ITurnManageable* requester, TSharedPtr<IBattleCommand>&& command)
{
  // コマンド実行中に登録禁止
  if (m_bIsExecutingCommand)
  {
    return;
  }

  check(IS_UINTERFACE_VALID(requester));
  if (!IS_UINTERFACE_VALID(requester))
  {
    return;
  }

  if (m_requestCmdWaitList.Contains(requester))
  {
    requester->SetCommandSendable(false);
    
    m_commandLists.Emplace(::MoveTemp(command));
    m_requestCmdWaitList.Remove(requester);
  }
  else
  {
    MDebug::LogError("Cant Register");
    return;
  }

  if (m_requestCmdWaitList.Num() == 0)
  {
    m_bIsExecutingCommand = true;
    if (OnStartExecuteEvent.IsBound())
    {
      OnStartExecuteEvent.Broadcast();
    }
  }
}

void UBattleCommandManager::ExecuteCommands(float deltaTime)
{
  if (!m_bIsExecutingCommand)
  {
    return;
  }

  for(int32 i = 0; i < m_commandLists.Num(); ++i)
  {
    const auto command = m_commandLists[i];
    if (command != nullptr)
    {
      command->Execute(deltaTime);
      if (command->IsFinish())
      {
        command->End();
        m_commandLists.RemoveAt(i);

        continue;
      }
    }
  }

  if (m_commandLists.Num() == 0)
  {
    m_bIsExecutingCommand = false;
    if (OnEndExecuteEvent.IsBound())
    {
      OnEndExecuteEvent.Broadcast();
    }
  }
}

void UBattleCommandManager::Test()
{
  MDebug::LogError("test tick battle command manager");
  if (OnEndExecuteEvent.IsBound())
  {
    OnEndExecuteEvent.Broadcast();
  }
}