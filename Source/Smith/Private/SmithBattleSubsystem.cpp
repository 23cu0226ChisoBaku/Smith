// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleSubsystem.h"
#include "ITurnManageable.h"
#include "TurnControlComponent.h"
#include "Kismet/GameplayStatics.h"
#include "IBattleCommand.h"

#include "TurnActor_Test.h"

bool USmithBattleSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
  return true;
}

void USmithBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  m_priorityLists.Reset();

  if (GEngine)
  {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, USmithBattleSubsystem::StaticClass()->GetName());
  }

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
    requestDelegate.BindUObject(this, &USmithBattleSubsystem::executeCommand);
    for (const auto manageable : turnManageable)
    {
      // TODO componentをInterfaceに変換
      UTurnControlComponent* turnCtrl = Cast<ITurnManageable>(manageable)->GetTurnControl();
      Cast<ITurnManageable>(manageable)->Subscribe(requestDelegate);
      if (turnCtrl != nullptr)
      {
        turnCtrl->SetCommandSendable(true);
        const ETurnPriority actorPriority = turnCtrl->GetPriority();

        if (!m_priorityLists.Contains(actorPriority))
        {
          m_priorityLists.Emplace(actorPriority, {});
        }
        
        m_priorityLists[actorPriority].Elements.Add(Cast<ITurnManageable>(manageable));
      }
    }
  }
}

void USmithBattleSubsystem::executeCommand(IBattleCommand* battleCommand)
{
  if (battleCommand == nullptr)
  {
    return;
  }

  battleCommand->Execute();
}
