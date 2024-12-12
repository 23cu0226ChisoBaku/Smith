// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActorGameMode_Test.h"
#include "SmithPlayerActor.h"
#include "SmithBattleSubsystem.h"

ASpawnActorGameMode_Test::ASpawnActorGameMode_Test()
{
  DefaultPawnClass = ASmithPlayerActor::StaticClass();
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
