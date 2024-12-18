// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithAttackComponent.h"
#include "IAttackable.h"

// Sets default values for this component's properties
USmithAttackComponent::USmithAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void USmithAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void USmithAttackComponent::Attack(AMyPlayerCharacter *player, int32 damage)
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Player to Attack");
	}
}

void USmithAttackComponent::SetAttackTarget(IAttackable* target)
{
	m_attackTarget = target;
}

void USmithAttackComponent::SetAttackHandle(AttackHandle&& handle)
{
	m_attackHandle = ::MoveTemp(handle);
}

void USmithAttackComponent::Attack_Temp()
{
	if (m_attackTarget.IsValid())
	{
		m_attackTarget->OnAttack(::MoveTemp(m_attackHandle));
		m_attackHandle = AttackHandle::NullHandle;
	}
}
