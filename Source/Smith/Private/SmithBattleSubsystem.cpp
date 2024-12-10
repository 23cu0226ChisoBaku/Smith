// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleSubsystem.h"
#include "SmithPlayerActor.h"

bool USmithBattleSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
  return true;
}

void USmithBattleSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  if (GEngine)
  {
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, USmithBattleSubsystem::StaticClass()->GetName());
  }

  // TODO  
  GetWorld()->SpawnActor<ASmithPlayerActor>(ASmithPlayerActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
}

void USmithBattleSubsystem::Deinitialize()
{

}