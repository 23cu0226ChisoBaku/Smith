// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithAttackComponent.h"

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
