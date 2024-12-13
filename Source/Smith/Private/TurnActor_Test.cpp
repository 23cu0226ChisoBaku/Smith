// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnActor_Test.h"
#include "TurnControlComponent.h"
#include "NullCommand.h"

namespace TEST_NS
{
  constexpr float SEND_INTERVAL = 3.0f;
}

ATurnActor_Test::ATurnActor_Test()
  : m_TimeCnt(0.0f)
{
  if (TurnComponent != nullptr)
  {
    TurnComponent->SetTurnPriority(ETurnPriority::Rival);
  }
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

  m_TimeCnt += DeltaTime;
  if (m_TimeCnt >= TEST_NS::SEND_INTERVAL)
  {
    m_TimeCnt = 0.0f;
    UE::Smith::Command::NullCommand nullCmd;
    SendCommand(&nullCmd);
  }
}
