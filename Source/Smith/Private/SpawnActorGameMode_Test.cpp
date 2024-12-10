// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnActorGameMode_Test.h"
#include "SmithPlayer_Turn.h"
#include "SmithPlayerActor.h"

ASpawnActorGameMode_Test::ASpawnActorGameMode_Test()
{
  DefaultPawnClass = ASmithPlayerActor::StaticClass();
}
