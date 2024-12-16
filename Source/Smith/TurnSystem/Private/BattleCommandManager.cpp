// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCommandManager.h"

#include "IBattleCommand.h"
#include "SmithBattleSubsystem.h"
#include "Debug.h"
#include "ZeroMem.h"

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

void UBattleCommandManager::FinishDestroy()
{
  Super::FinishDestroy();

  //ZeroMemory_Class();
}

void UBattleCommandManager::RegisterCommand(IBattleCommand* command)
{
  // コマンド実行中に登録禁止
  if (m_bIsExecutingCommand)
  {
    return;
  }

  m_commandLists.Add(command);
}

void UBattleCommandManager::ExecuteCommands(float deltaTime)
{
  for(auto command : m_commandLists)
  {
    if (command != nullptr)
    {
      command->Execute(deltaTime);
    }
  }
}

void UBattleCommandManager::Test()
{
  UE::MLibrary::Debug::LogError("test tick battle command manager");
  if (OnEndExecuteEvent.IsBound())
  {
    OnEndExecuteEvent.Broadcast();
  }
}