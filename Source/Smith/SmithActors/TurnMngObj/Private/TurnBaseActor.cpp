// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBaseActor.h"
#include "IMoveable.h"
#include "ICommandMediator.h"
#include "MoveDirection.h"
#include "SmithAIBehaviorProcessor.h"
#include "SmithAIStrategy.h"

#include "MLibrary.h"

// Sets default values
ATurnBaseActor::ATurnBaseActor()
	: m_aiBehaviorProcessor(nullptr)
	, m_commandMediator(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurnBaseActor::BeginPlay()
{
	Super::BeginPlay();

	if (bUseSmithAIProcessor)
	{
		m_aiBehaviorProcessor = NewObject<USmithAIBehaviorProcessor>(this);
		check(m_aiBehaviorProcessor != nullptr);

		m_aiBehaviorProcessor->TickConditionDelegate.BindUObject(this, &ATurnBaseActor::IsCommandSendable);
	}

}

// Called every frame
void ATurnBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurnBaseActor::SetCommandMediator(ICommandMediator* mediator)
{
	m_commandMediator = mediator;
}

void ATurnBaseActor::SendMoveCommand(IMoveable* moveable, UE::Smith::Battle::EMoveDirection direction, uint8 moveDistance)
{
	if (!IsCommandSendable())
	{
		return;
	}
	else
	{
		MDebug::LogWarning("send succeed");
	}

	if (m_commandMediator.IsValid())
	{
		m_commandMediator->SendMoveCommand(this, moveable, direction, moveDistance);
	}
}

void ATurnBaseActor::SendAttackCommand(ICanMakeAttack* attacker, UE::Smith::Battle::EMoveDirection direction, const UE::Smith::Battle::FSmithCommandFormat& format, AttackHandle&& handle)
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_commandMediator.IsValid())
	{
		m_commandMediator->SendAttackCommand(this, attacker, direction, format, ::MoveTemp(handle));
	}
}
