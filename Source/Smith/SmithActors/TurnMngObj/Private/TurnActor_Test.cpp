// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnActor_Test.h"

ATurnActor_Test::ATurnActor_Test()
{
  SetTurnPriority(ETurnPriority::Rival);
}

void ATurnActor_Test::BeginPlay()
{
  Super::BeginPlay();
}

void ATurnActor_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}

void ATurnActor_Test::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  SendMoveCommand(nullptr);
}
