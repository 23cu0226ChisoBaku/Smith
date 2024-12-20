// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleSubsystem.h"
#include "ITurnManageable.h"
#include "TurnControlComponent.h"
#include "Kismet/GameplayStatics.h"
#include "IBattleCommand.h"
#include "BattleCommandManager.h"

#include "Debug.h"
#include "TurnActor_Test.h"

bool USmithBattleSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
  return true;
}

void USmithBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);
  m_bCanExecuteCmd = false;

  emptyContainers();
  if (m_battleCmdMgr == nullptr)
  {
    m_battleCmdMgr = NewObject<UBattleCommandManager>(GetWorld());
  }

  check(m_battleCmdMgr != nullptr)

  m_battleCmdMgr->OnStartExecuteEvent.AddUObject(this, &USmithBattleSubsystem::startExecute);
  m_battleCmdMgr->OnEndExecuteEvent.AddUObject(this, &USmithBattleSubsystem::endExecute);
  
}

void USmithBattleSubsystem::Deinitialize()
{
  Super::Deinitialize();
  emptyContainers();
  if (m_battleCmdMgr != nullptr)
  {
    m_battleCmdMgr->MarkAsGarbage();
  }
}

void USmithBattleSubsystem::StartBattle()
{
  TArray<AActor*> turnManageable;
  UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTurnManageable::StaticClass(),turnManageable);

  if (turnManageable.Num() > 0)
  {
    FRequestCommandEvent::FDelegate requestDelegate;
    requestDelegate.BindUObject(this, &USmithBattleSubsystem::registerCommand);

    TArray<TWeakInterfacePtr<ITurnManageable>> registerWaitList;
    registerWaitList.Reserve(turnManageable.Num());

    for (const auto manageable : turnManageable)
    {
      // TODO componentをInterfaceに変換
      ITurnManageable* iManageable = Cast<ITurnManageable>(manageable);
      UTurnControlComponent* turnCtrl = iManageable->GetTurnControl();
      iManageable->Subscribe(requestDelegate);

      if (turnCtrl != nullptr)
      {
        const ETurnPriority actorPriority = turnCtrl->GetPriority();
        if (actorPriority == ETurnPriority::PlayerSelf)
        {
          turnCtrl->SetCommandSendable(true);
          registerWaitList.Emplace(iManageable);
        }
        else
        {
          turnCtrl->SetCommandSendable(false);
        }

        if (!m_priorityManageableLists.Contains(actorPriority))
        {
          m_priorityManageableLists.Emplace(actorPriority, {});
        }
        
        m_priorityManageableLists[actorPriority].Elements.Add(Cast<ITurnManageable>(manageable));
      }
    }

    if (m_battleCmdMgr != nullptr)
    {
      m_battleCmdMgr->RegisterWaitList(registerWaitList);
    }

    m_curtTurn = ETurnPriority::PlayerSelf;
  }
}

void USmithBattleSubsystem::registerCommand(ITurnManageable* requester, TSharedPtr<IBattleCommand> battleCommand)
{
  if (requester == nullptr || battleCommand == nullptr)
  {
    return;
  }

  check(m_battleCmdMgr != nullptr);

  if (m_battleCmdMgr == nullptr)
  {
    return;
  }
  m_battleCmdMgr->RegisterCommand(requester, ::MoveTemp(battleCommand));

}

void USmithBattleSubsystem::registerNextTurnObjs()
{
  // TODO Strange Design
  uint8 errorCnt = 0;

  const ETurnPriority prevTurn = m_curtTurn;
  do
  {
    m_curtTurn = StaticCast<ETurnPriority>((StaticCast<uint8>(m_curtTurn) + 1u) % StaticCast<uint8>(ETurnPriority::PriorityTypeCnt));

    if (m_curtTurn == prevTurn)
    {
      break;
    }

  } while (!m_priorityManageableLists.Contains(m_curtTurn));

  if (m_battleCmdMgr != nullptr)
  {
    int32 idx = 0;
    while (idx < m_priorityManageableLists[m_curtTurn].Elements.Num())
    {
      auto nextTurnObj = m_priorityManageableLists[m_curtTurn].Elements[idx];
      bool invalid = false;
      if (nextTurnObj.IsValid())
      {
        auto turnCtrl = nextTurnObj->GetTurnControl();
        if (::IsValid(turnCtrl))
        {
          turnCtrl->SetCommandSendable(true);
        }
        else
        {
          invalid = true;
        }
      }
      else
      {
        invalid = true;
      }

      if (invalid)
      {
        m_priorityManageableLists[m_curtTurn].Elements.RemoveAt(idx);
        continue;
      }

      ++idx;
    }
    m_battleCmdMgr->RegisterWaitList(m_priorityManageableLists[m_curtTurn].Elements);
  }
  
}

void USmithBattleSubsystem::emptyContainers()
{
  m_priorityManageableLists.Empty();
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

  if (m_bCanExecuteCmd) 
  {
    if (m_battleCmdMgr != nullptr)
    {
      m_battleCmdMgr->ExecuteCommands(DeltaTime);
    }
  }


  if (m_priorityManageableLists.Contains(ETurnPriority::Rival) && m_priorityManageableLists[ETurnPriority::Rival].Elements.Num() == 0)
  {
   
    UGameplayStatics::OpenLevel(GetWorld(), FName(*GetWorld()->GetName()), false);
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
