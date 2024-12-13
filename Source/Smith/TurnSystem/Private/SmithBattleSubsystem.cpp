// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleSubsystem.h"
#include "ITurnManageable.h"
#include "TurnControlComponent.h"
#include "Kismet/GameplayStatics.h"
#include "IBattleCommand.h"

#include "Debug.h"
#include "TurnActor_Test.h"

bool USmithBattleSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
  return true;
}

void USmithBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  m_priorityLists.Empty();
  m_requestCmdWaitList.Empty();
  m_cmdExecuteQueue.Empty();

  // TODO  
  //GetWorld()->SpawnActor<ASmithPlayerActor>(ASmithPlayerActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
  GetWorld()->SpawnActor<ATurnActor_Test>(ATurnActor_Test::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
}

void USmithBattleSubsystem::Deinitialize()
{
  m_priorityLists.Reset();
}

void USmithBattleSubsystem::StartBattle()
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
      UTurnControlComponent* turnCtrl = Cast<ITurnManageable>(manageable)->GetTurnControl();
      Cast<ITurnManageable>(manageable)->Subscribe(requestDelegate);
      if (turnCtrl != nullptr)
      {
        const ETurnPriority actorPriority = turnCtrl->GetPriority();
        if (actorPriority == ETurnPriority::PlayerSelf)
        {
          turnCtrl->SetCommandSendable(true);
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

void USmithBattleSubsystem::registerCommand(TSharedPtr<IBattleCommand> battleCommand)
{
  if (battleCommand == nullptr)
  {
    return;
  }

  m_cmdExecuteQueue.Enqueue(battleCommand);

  // if (m_requestCmdWaitList.Num() == 0)
  // {
    executeCommand();
  // }
}

void USmithBattleSubsystem::executeCommand()
{
  TSharedPtr<IBattleCommand> cmd;
  while(m_cmdExecuteQueue.Dequeue(cmd))
  {
    cmd->Execute();
  }

  m_cmdExecuteQueue.Empty();

  if (m_priorityLists.Contains(m_curtTurn))
  {
    for(ITurnManageable* prevTurnObj :m_priorityLists[m_curtTurn].Elements)
    {
      UTurnControlComponent* turnCtrl = prevTurnObj->GetTurnControl();
      if (::IsValid(turnCtrl))
      {
        turnCtrl->SetCommandSendable(false);
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

  for(ITurnManageable* prevTurnObj : m_priorityLists[m_curtTurn].Elements)
  {
    UTurnControlComponent* turnCtrl = prevTurnObj->GetTurnControl();
    if (::IsValid(turnCtrl))
    {
      turnCtrl->SetCommandSendable(true);
    }
    m_requestCmdWaitList.Add(prevTurnObj);
  }
  
}
