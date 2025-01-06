// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnActor_Test.h"
#include "AttackHandle.h"
#include "MLibrary.h"
#include "MoveDirection.h"

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
  SendMoveCommand(nullptr, UE::Smith::Battle::EMoveDirection::None, 0);
}

void ATurnActor_Test::OnAttack(AttackHandle&& handle)
{
  MDebug::LogError("Get Attack by" + handle.AttackName);
}

uint8 ATurnActor_Test::GetOnMapSizeX() const
{
  return 1;
}

uint8 ATurnActor_Test::GetOnMapSizeY() const
{
  return 1;
}
