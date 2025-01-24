// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "SmithAnimationComponent.h"
#include "Debug.h"

// Sets default values for this component's properties
USmithAnimationComponent::USmithAnimationComponent()
	: m_curtAnimationTimeInterval(0.0f)
	, m_animationPlayTimeCnt(0.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USmithAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	MontageToPlay = LoadObject<UAnimMontage>(nullptr, *objectPass);
	AActor* OwnerActor = GetOwner();
	if (OwnerActor != nullptr)
	{
		USkeletalMeshComponent* skeltalmeshcomp = OwnerActor->FindComponentByClass<USkeletalMeshComponent>();
		if (skeltalmeshcomp)
		{
			AnimInstance = skeltalmeshcomp->GetAnimInstance();
		}
	}

	if (AnimInstance != nullptr && MontageToPlay != nullptr)
	{
		AnimInstance->Montage_Play(MontageToPlay);
	}

}


// Called every frame
void USmithAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	return;
	// ...
}

void USmithAnimationComponent::SwitchAnimState(FName nextStateName, float animationDuration)
{
	if (AnimInstance == nullptr)
	{
		return;
	}

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage == nullptr)
	{
		return;
	}

	FName CurrentSectionName = AnimInstance->Montage_GetCurrentSection(CurrentMontage);

	if (CurrentSectionName == nextStateName) 
	{ 
		return; 
	}

	m_curtAnimationTimeInterval = animationDuration;
	m_animationPlayTimeCnt = 0.0f;

	AnimInstance->Montage_Play(MontageToPlay);
	AnimInstance->Montage_JumpToSection(nextStateName, MontageToPlay);
}

void USmithAnimationComponent::UpdateAnim(float deltaTime)
{
	m_animationPlayTimeCnt += deltaTime;
}

bool USmithAnimationComponent::IsCurrentAnimationFinish() const
{
	return m_animationPlayTimeCnt >= m_curtAnimationTimeInterval;
}

void USmithAnimationComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

}

