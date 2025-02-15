// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithAttackComponent.h"
#include "IAttackable.h"
#include "SmithTestAIAttackStrategy.h"
#include "SmithCommandFormat.h"
#include "AudioKit.h"

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

	// if (AIStrategySubclass != nullptr)
	// {
	// 	m_aiStrategy = NewObject<USmithAIAttackStrategy>(this, AIStrategySubclass);
	// }


	// if (m_aiStrategy != nullptr)
	// {
	// 	for (const auto& pair : AttackFormatTables)
	// 	{
	// 		if (!pair.Value.IsValid())
	// 		{
	// 			pair.Value.LoadSynchronous();
	// 		}

	// 		UDataTable* formatTable = pair.Value.Get();
	// 		if (!::IsValid(formatTable))
	// 		{
	// 			continue;
	// 		}

	// 		m_aiStrategy->RegisterAttackFormat(pair.Key, formatTable);
	// 	}
	// }

	// ...
}

void USmithAttackComponent::SetAttackTarget(IAttackable* target)
{
	m_attackTarget = target;
}

void USmithAttackComponent::SetAttackHandle(AttackHandle&& handle)
{
	m_attackHandle = ::MoveTemp(handle);
}

void USmithAttackComponent::Attack()
{
	if (m_attackTarget.IsValid())
	{
		m_attackTarget->OnAttack(::MoveTemp(m_attackHandle));
		m_attackHandle = AttackHandle::NullHandle;
		MLibrary::UE::Audio::AudioKit::PlaySE(AttackSEName);
	}

}
