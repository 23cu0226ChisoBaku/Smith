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
  cnt = 0.0f;

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

void USmithBattleSubsystem::RegisterTurnObj()
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

        if (!m_priorityLists.Contains(actorPriority))
        {
          m_priorityLists.Emplace(actorPriority, {});
        }
        
        m_priorityLists[actorPriority].Elements.Add(Cast<ITurnManageable>(manageable));
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
  do
  {
    #ifdef WITH_EDITOR
      ++errorCnt;
      if (errorCnt > StaticCast<uint8>(ETurnPriority::PriorityTypeCnt))
      {
        MDebug::LogError(TEXT("registerNextTurnObject Error!!!"));
        check(false);
      }
    #endif
    m_curtTurn = StaticCast<ETurnPriority>((StaticCast<uint8>(m_curtTurn) + 1u) % StaticCast<uint8>(ETurnPriority::PriorityTypeCnt));
  
  } while (!m_priorityLists.Contains(m_curtTurn));

  if (m_battleCmdMgr != nullptr)
  {
    for(auto prevTurnObj : m_priorityLists[m_curtTurn].Elements)
    {
      if (prevTurnObj.IsValid())
      {
        auto turnCtrl = prevTurnObj->GetTurnControl();
        if (::IsValid(turnCtrl))
        {
          turnCtrl->SetCommandSendable(true);
        }
      }
    }

    m_battleCmdMgr->RegisterWaitList(m_priorityLists[m_curtTurn].Elements);
  }
  
}

void USmithBattleSubsystem::emptyContainers()
{
  m_priorityLists.Empty();
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

  cnt += DeltaTime;
  if (m_bCanExecuteCmd) 
  {
    if (m_battleCmdMgr != nullptr)
    {
      m_battleCmdMgr->ExecuteCommands(DeltaTime);
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
