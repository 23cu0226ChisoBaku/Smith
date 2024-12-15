// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleCommandManager.h"
#include "IBattleCommand.h"
#include "SmithBattleSubsystem.h"
#include "Debug.h"

UBattleCommandManager::UBattleCommandManager(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{}

void UBattleCommandManager::Tick(float DeltaTime)
{
  if (m_bIsExecutingCommand)
  {
    UE::MLibrary::Debug::LogWarning(FString::SanitizeFloat(DeltaTime));
    executeCommands();
    resetCommands();
  }
}

bool UBattleCommandManager::IsTickable() const
{
  if (GetWorld() != nullptr)
  {
    USmithBattleSubsystem* battleSub = GetWorld()->GetSubsystem<USmithBattleSubsystem>();
    if (battleSub != nullptr)
    {
      return true;
    }
    // return m_commandLists.Num() > 0;

  }
  return false;
}

TStatId UBattleCommandManager::GetStatId() const
{
  RETURN_QUICK_DECLARE_CYCLE_STAT(UBattleCommandManager, STATGROUP_Tickables);
}

bool UBattleCommandManager::IsTickableWhenPaused() const
{
  return false;
}

bool UBattleCommandManager::IsTickableInEditor() const
{
  return false;
}

UWorld* UBattleCommandManager::GetTickableGameObjectWorld() const
{
  return GetWorld();
}

UWorld* UBattleCommandManager::GetWorld() const
{
  UWorld* world = nullptr;
  if(GetOuter() != nullptr)
  {
    world = GetOuter()->GetWorld();
  }

  if (world == nullptr)
  {
    world = GWorld;
  }

  return world;

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

void UBattleCommandManager::StartCommands()
{
  // コマンドがないと開始しない
  if (m_commandLists.Num() == 0)
  {
    return;
  }

  m_bIsExecutingCommand = true;
}

void UBattleCommandManager::executeCommands()
{
  for(auto command : m_commandLists)
  {
    if (command != nullptr)
    {
      command->Execute();
    }
  }


}

void UBattleCommandManager::resetCommands()
{
  m_bIsExecutingCommand = false;
  m_commandLists.Empty();
}