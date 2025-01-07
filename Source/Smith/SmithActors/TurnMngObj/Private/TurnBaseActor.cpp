// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBaseActor.h"
#include "IMoveable.h"
#include "ICommandMediator.h"

// Sets default values
ATurnBaseActor::ATurnBaseActor()
	: m_commandMediator(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurnBaseActor::BeginPlay()
{
	Super::BeginPlay();
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

void ATurnBaseActor::SendMoveCommand(IMoveable* moveable)
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_commandMediator.IsValid())
	{
		m_commandMediator->SendMoveCommand(this, moveable);
	}
}

void ATurnBaseActor::SendAttackCommand(ICanMakeAttack* attacker, IAttackable* target, AttackHandle&& handle)
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_commandMediator.IsValid())
	{
		m_commandMediator->SendAttackCommand(this, attacker, target, ::MoveTemp(handle));
	}
}

void ATurnBaseActor::SendSkillCommand(ISkillable* skillable)
{
	if (!IsCommandSendable())
	{
		return;
	}

	if (m_commandMediator.IsValid())
	{
		m_commandMediator->SendSkillCommand(this, skillable);
	}
}
