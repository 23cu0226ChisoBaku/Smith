// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnActor_Test.h"
#include "AttackHandle.h"
#include "MoveDirection.h"
#include "SmithAIRegistry.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategyContainer.h"
#include "SmithTurnBaseAIAttackStrategy.h"
#include "SmithAttackComponent.h"
#include "FormatInfo_Import.h"
#include "MLibrary.h"

ATurnActor_Test::ATurnActor_Test()
{
  PrimaryActorTick.bCanEverTick = false;
  SetTurnPriority(ETurnPriority::Rival);

  m_atkComponent = CreateDefaultSubobject<USmithAttackComponent>(TEXT("attack comp test"));
  check(m_atkComponent != nullptr);

}

void ATurnActor_Test::BeginPlay()
{
  Super::BeginPlay();

  m_attackStrategy = NewObject<USmithTurnBaseAIAttackStrategy>(this);
  if (m_attackStrategy != nullptr)
  {
    m_attackStrategy->SetOwner(this);
    m_attackStrategy->Initialize(m_atkComponent, m_commandMediator.Get());
  }
  // TODO
	for (auto& pair : AttackFormatTables)
	{
		if (!pair.Value.IsValid())
		{
			pair.Value.LoadSynchronous();
		}

    if (m_attackStrategy != nullptr)
    {
	  	m_attackStrategy->RegisterAttackFormat(pair.Key, pair.Value.Get());
    }
	}

  if (m_aiBehaviorProcessor != nullptr)
  {  
    if (m_attackStrategy != nullptr)
    {
      m_aiBehaviorProcessor->RegisterAIStrategy(0, m_attackStrategy);
    }

		m_aiBehaviorProcessor->RunBehaviorProcessor();
  }
}

void ATurnActor_Test::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}

void ATurnActor_Test::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  //SendMoveCommand(nullptr, UE::Smith::Battle::EMoveDirection::None, 0);
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
