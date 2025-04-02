// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "ITurnManageable.h"
#include "IBattleCommand.h"
#include "BattleCommandManager.h"
#include "IEventExecutor.h"
#include "SmithTurnBattleWorldSettings.h"

#include "MLibrary.h"

bool USmithBattleSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
  if (!Super::ShouldCreateSubsystem(Outer))
  {
    return false;
  }

  UWorld* worldOuter = Cast<UWorld>(Outer);
  if (::IsValid(worldOuter))
  {
    // バトルレベルだけ作る
    ASmithTurnBattleWorldSettings* smithWorldSettings = Cast<ASmithTurnBattleWorldSettings>(worldOuter->GetWorldSettings());
    if (::IsValid(smithWorldSettings))
    {
      return smithWorldSettings->IsBattleLevel();
    }
  }

  return false;
}

void USmithBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);
  if (m_battleCmdMgr == nullptr)
  {
    m_battleCmdMgr = NewObject<UBattleCommandManager>(GetWorld());
  }
  check(m_battleCmdMgr != nullptr)
  ResetBattle();

  m_startDelegateHandle = m_battleCmdMgr->OnStartExecuteEvent.AddUObject(this, &USmithBattleSubsystem::startExecute);
  m_endDelegateHandle = m_battleCmdMgr->OnEndExecuteEvent.AddUObject(this, &USmithBattleSubsystem::endExecute);
  m_bIsInitialized = true;
}

void USmithBattleSubsystem::Deinitialize()
{
  m_bIsInitialized = false;
  ResetBattle();
  if (m_battleCmdMgr != nullptr)
  {
    m_battleCmdMgr->OnStartExecuteEvent.Remove(m_startDelegateHandle);
    m_battleCmdMgr->OnEndExecuteEvent.Remove(m_endDelegateHandle);
    m_battleCmdMgr->MarkAsGarbage();
  }

  Super::Deinitialize();
}

void USmithBattleSubsystem::InitializeBattle()
{
  // ターン管理オブジェクトを全て登録
  TArray<AActor*> turnManageable;
  UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTurnManageable::StaticClass(),turnManageable);

  if (turnManageable.Num() > 0)
  {
    TArray<TWeakInterfacePtr<ITurnManageable>> registerWaitList;
    registerWaitList.Reserve(turnManageable.Num());

    for (const auto& manageable : turnManageable)
    {
      ITurnManageable* manageInterface = Cast<ITurnManageable>(manageable);
      const ETurnPriority actorPriority = manageInterface->GetPriority();

      // プレイヤーだったら先に行動できる
      if (actorPriority == ETurnPriority::PlayerSelf)
      {
        manageInterface->SetCommandSendable(true);
        registerWaitList.Emplace(manageable);
      }
      else
      {
        manageInterface->SetCommandSendable(false);
      }

      if (!m_priorityManageableLists.Contains(actorPriority))
      {
        m_priorityManageableLists.Emplace(actorPriority, {});
      }
      
      m_priorityManageableLists[actorPriority].Add(manageInterface);
    }

    if (m_battleCmdMgr != nullptr)
    {
      m_battleCmdMgr->Initialize();
      m_battleCmdMgr->RegisterWaitList(registerWaitList);
    }

    m_curtTurn = ETurnPriority::PlayerSelf;
  }
}

void USmithBattleSubsystem::ResetBattle()
{
  m_battleCmdMgr->Reset();
  m_priorityManageableLists.Reset();
  m_bCanExecuteCmd = false;
}

void USmithBattleSubsystem::AssignEventExecutor(IEventExecutor* eventExecutor)
{
  if (m_battleCmdMgr != nullptr)
  {
    m_battleCmdMgr->AssignEventExecutor(eventExecutor);
  }
}

void USmithBattleSubsystem::RegisterCommand(ITurnManageable* requester, TSharedPtr<IBattleCommand> battleCommand)
{
  check(m_battleCmdMgr != nullptr);
  if (m_battleCmdMgr == nullptr)
  {
    return;
  }

  if (requester == nullptr || battleCommand == nullptr)
  {
    MDebug::LogError("Can not register!!!");
    return;
  }

  m_battleCmdMgr->RegisterCommand(requester, ::MoveTemp(battleCommand));

}

void USmithBattleSubsystem::registerNextTurnObjs()
{
  check(m_battleCmdMgr != nullptr);
  if (m_battleCmdMgr == nullptr)
  {
    return;
  }

  const ETurnPriority prevTurn = m_curtTurn;
  ETurnPriority nextTurn = prevTurn;
  do
  {
    nextTurn = StaticCast<ETurnPriority>((StaticCast<uint8>(nextTurn) + 1u) % StaticCast<uint8>(ETurnPriority::PriorityTypeCnt));
    if (m_priorityManageableLists.Contains(nextTurn))
    {
      int32 idx = 0;
    // 次のターンのオブジェクトへコマンド送信許可を下す
      while (idx < m_priorityManageableLists[nextTurn].Num())
      {
        auto nextTurnObj = m_priorityManageableLists[nextTurn][idx];
        bool invalid = false;

        if (nextTurnObj.IsValid())
        {
          nextTurnObj->SetCommandSendable(true);
        }
        else
        {
          invalid = true;
        }

        if (invalid)
        {
          m_priorityManageableLists[nextTurn].RemoveAt(idx);
          continue;
        }
        ++idx;
      }

      // コマンド待ちリストを設定
      if (m_priorityManageableLists[nextTurn].Num() > 0)
      {
        m_battleCmdMgr->RegisterWaitList(m_priorityManageableLists[nextTurn]);
        m_curtTurn = nextTurn;
        break;
      }
    }
  } while (nextTurn != prevTurn);
}

void USmithBattleSubsystem::startExecute()
{
  check(((m_battleCmdMgr != nullptr) && (!m_bCanExecuteCmd)))
  m_bCanExecuteCmd = true;
}

void USmithBattleSubsystem::endExecute()
{
  check(((m_battleCmdMgr != nullptr) && (m_bCanExecuteCmd)))
  m_bCanExecuteCmd = false;
  registerNextTurnObjs();
}

void USmithBattleSubsystem::Tick(float DeltaTime)
{
  UTickableWorldSubsystem::Tick(DeltaTime);
  
  if (!m_bIsInitialized)
  {
    return;
  }

  if (m_battleCmdMgr != nullptr)
  {
    if (m_bCanExecuteCmd) 
    {
      m_battleCmdMgr->ExecuteCommands(DeltaTime);
    }
    else
    {
      m_battleCmdMgr->ForceWaitListValidation();
    }
  }
}

bool USmithBattleSubsystem::IsTickable() const
{
  return true;
}

TStatId USmithBattleSubsystem::GetStatId() const
{
  RETURN_QUICK_DECLARE_CYCLE_STAT(USmithBattleSubsystem, STATGROUP_Tickables);
}

bool USmithBattleSubsystem::IsTickableWhenPaused() const
{
  return false;
}

bool USmithBattleSubsystem::IsTickableInEditor() const
{
  return false;
}

UWorld* USmithBattleSubsystem::GetTickableGameObjectWorld() const
{
  return GetWorld();
}
