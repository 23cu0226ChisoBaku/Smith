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
  emptyContainers();

  // TODO  
  //GetWorld()->SpawnActor<ASmithPlayerActor>(ASmithPlayerActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
  for (int i = 0; i < 2; ++i)
  {
    GetWorld()->SpawnActor<ATurnActor_Test>(ATurnActor_Test::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
  }

  
}

void USmithBattleSubsystem::Deinitialize()
{
  emptyContainers();
}

void USmithBattleSubsystem::RegisterTurnObj()
{
  TArray<AActor*> turnManageable;
  UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTurnManageable::StaticClass(),turnManageable);

  if (turnManageable.Num() > 0)
  {
    FRequestCommandEvent::FDelegate requestDelegate;
    requestDelegate.BindUObject(this, &USmithBattleSubsystem::registerCommand);

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
          m_requestCmdWaitList.Emplace(iManageable);
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

    m_curtTurn = ETurnPriority::PlayerSelf;
  }
}

void USmithBattleSubsystem::registerCommand(ITurnManageable* requester, TSharedPtr<IBattleCommand> battleCommand)
{
  if (battleCommand == nullptr)
  {
    return;
  }

  if (m_requestCmdWaitList.Contains(requester))
  {
    m_requestCmdWaitList.Remove(requester);
    m_cmdExecuteQueue.Enqueue(battleCommand);
  }

  if (m_requestCmdWaitList.Num() == 0)
  {
    executeCommand();
  }
}

void USmithBattleSubsystem::executeCommand()
{
  TSharedPtr<IBattleCommand> cmd;
  while(m_cmdExecuteQueue.Dequeue(cmd))
  {
    if (cmd != nullptr)
    {
      cmd->Execute();
    }
    else
    {
      UE::MLibrary::Debug::LogError("command nullptr");
    }
  }

  if (m_priorityLists.Contains(m_curtTurn))
  {
    for(const auto& prevTurnObj :m_priorityLists[m_curtTurn].Elements)
    {
      if (prevTurnObj.IsValid())
      {
        UTurnControlComponent* turnCtrl = prevTurnObj->GetTurnControl();
        if (::IsValid(turnCtrl))
        {
          turnCtrl->SetCommandSendable(false);
        }
      }
    }
  }

  registerNextTurnObjs();
}

void USmithBattleSubsystem::registerNextTurnObjs()
{
  uint8 errorCnt = 0;
  do
  {
    #ifdef WITH_EDITOR
      ++errorCnt;
      if (errorCnt > StaticCast<uint8>(ETurnPriority::PriorityTypeCnt))
      {
        UE::MLibrary::Debug::LogError(TEXT("registerNextTurnObject Error!!!"));
        //check(false);
      }
    #endif
    m_curtTurn = StaticCast<ETurnPriority>((StaticCast<uint8>(m_curtTurn) + 1u) % StaticCast<uint8>(ETurnPriority::PriorityTypeCnt));
  
  } while (!m_priorityLists.Contains(m_curtTurn));

  m_requestCmdWaitList.Empty();

  for(const auto& prevTurnObj : m_priorityLists[m_curtTurn].Elements)
  {
    if (prevTurnObj.IsValid())
    {
      UTurnControlComponent* turnCtrl = prevTurnObj->GetTurnControl();
      if (::IsValid(turnCtrl))
      {
        turnCtrl->SetCommandSendable(true);
      }

      m_requestCmdWaitList.Add(prevTurnObj.Get());
    }
  }
  
}

void USmithBattleSubsystem::emptyContainers()
{
  m_priorityLists.Empty();
  m_requestCmdWaitList.Empty();
  m_cmdExecuteQueue.Empty();
}
