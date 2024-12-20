// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnControlComponent.h"
#include "Debug.h"

// Sets default values for this component's properties
UTurnControlComponent::UTurnControlComponent()
	: m_priority(ETurnPriority::PlayerSelf)
	, m_isCMDSendable(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UTurnControlComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("turn component added"));
	}
	// ...
	
}

void UTurnControlComponent::SetCommandSendable(bool value)
{
	m_isCMDSendable = value;

	// FString str {TEXT("Turn ")};
	// str.Append(GetOwner()->GetName());
	// str.Append(" ");
	// str.Append(m_isCMDSendable ? "On" : "Off");
	// UE::MLibrary::Debug::LogWarning(str);
}

void UTurnControlComponent::SetTurnPriority(ETurnPriority priority)
{
	m_priority = priority;
}

bool UTurnControlComponent::IsCommandSendable() const
{
	return m_isCMDSendable;
}

ETurnPriority UTurnControlComponent::GetPriority() const
{
	return m_priority;
}

