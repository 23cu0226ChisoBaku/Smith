// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActorGameMode_Test.h"
#include "SmithPlayerActor.h"
#include "SmithBattleSubsystem.h"

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

  USmithBattleSubsystem* subsys = GetWorld()->GetSubsystem<USmithBattleSubsystem>();
  if (subsys != nullptr)
  {
    subsys->StartBattle();
  }
}
