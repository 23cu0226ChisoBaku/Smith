// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCommandManager.h"
#include "IBattleCommand.h"
#include "SmithBattleSubsystem.h"
#include "IEventExecutor.h"
#include "MLibrary.h"

UBattleCommandManager::UBattleCommandManager(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_requestCmdWaitList{}
  , m_commandLists{}
  , m_eventExecutor(nullptr)
  , m_bIsExecutingCommand(false)
  , m_bCanRegister(false)
{}

void UBattleCommandManager::BeginDestroy()
{
  Super::BeginDestroy();

  OnStartExecuteEvent.Clear();
  OnEndExecuteEvent.Clear();

  Reset();
}

void UBattleCommandManager::AssignEventExecutor(IEventExecutor* eventExecutor)
{
  m_eventExecutor = eventExecutor;
}

void UBattleCommandManager::RegisterWaitList(const TArray<TWeakInterfacePtr<ITurnManageable>>& waitList)
{
  for (const auto& waitObj : waitList)
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
  if (m_bIsExecutingCommand || !m_bCanRegister)
  {
    MDebug::LogError("Cant register");
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

void UBattleCommandManager::ExecuteCommands(float deltaTime)
{
  if (!m_bIsExecutingCommand)
  {
    return;
  }

  for(int32 i = 0; i < m_commandLists.Num(); ++i)
  {
    const auto& command = m_commandLists[i];
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

    if (m_eventExecutor.IsValid() && m_eventExecutor->IsEventInStock())
    { 
      m_eventExecutor->ExecuteEvent();
    }
  }
}
void UBattleCommandManager::Initialize()
{
  m_bCanRegister = true;
}
void UBattleCommandManager::Reset()
{
  m_bIsExecutingCommand = false;
  m_bCanRegister = false;
  m_requestCmdWaitList.Reset();
  m_commandLists.Reset();
}
