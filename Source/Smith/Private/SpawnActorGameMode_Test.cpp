// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActorGameMode_Test.h"
#include "SmithPlayerActor.h"
#include "SmithBattleSubsystem.h"
#include "TurnActor_Test.h"

#include "MyEnemy.h"

ASpawnActorGameMode_Test::ASpawnActorGameMode_Test()
{
  static ConstructorHelpers::FClassFinder<APawn> PlayerBP(TEXT("/Game/BP/BP_SmithPlayerActor"));
  if (PlayerBP.Class != nullptr)
  {
    DefaultPawnClass = PlayerBP.Class;
  }
}

void ASpawnActorGameMode_Test::StartPlay()
{
  Super::StartPlay();

  for (int i = 0; i < 2; ++i)
  {
    //GetWorld()->SpawnActor<ATurnActor_Test>(ATurnActor_Test::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
  }

  //GetWorld()->SpawnActor<AMyEnemy>(AMyEnemy::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);


  USmithBattleSubsystem* subsys = GetWorld()->GetSubsystem<USmithBattleSubsystem>();
  if (subsys != nullptr)
  {
    subsys->RegisterTurnObj();
  }
}
