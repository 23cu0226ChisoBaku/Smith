// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithAnimationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Debug.h"
#include "AnimMontageHelperLibrary.h"

// Sets default values for this component's properties
USmithAnimationComponent::USmithAnimationComponent()
	: m_curtAnimationTimeInterval(0.0f)
	, m_animationPlayTimeCnt(0.0f)
	, m_animationSwitchDelayTimeInterval(0.0f)
	, m_animationSwitchDelayTimeCnt(0.0f)
	, m_delayNextSectionName()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

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
	
	// ...
	if (m_delayNextSectionName.IsNone())
	{
		return;
	}

	m_animationSwitchDelayTimeCnt += DeltaTime;
	if (m_animationSwitchDelayTimeCnt >= m_animationSwitchDelayTimeInterval)
	{
		SwitchAnimState(m_delayNextSectionName);
	}

}

void USmithAnimationComponent::SwitchAnimState(FName nextStateName)
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

	const float duration = UAnimMontageHelperLibrary::GetSectionDuration(AnimInstance, CurrentMontage, nextStateName);

	MDebug::LogWarning(FString::SanitizeFloat(duration));

	m_curtAnimationTimeInterval = duration;

	m_animationPlayTimeCnt = 0.0f;

	// delayがあったら
	if (m_animationSwitchDelayTimeInterval > 0.0f)
	{
		m_animationSwitchDelayTimeInterval = 0.0f;
		m_animationSwitchDelayTimeCnt = 0.0f;
		m_delayNextSectionName = NAME_None;

		SetComponentTickEnabled(false);
	}

	AnimInstance->Montage_Play(MontageToPlay);
	AnimInstance->Montage_JumpToSection(nextStateName, MontageToPlay);
}

void USmithAnimationComponent::SwitchAnimStateDelay(FName nextStateName, float delay)
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

	m_delayNextSectionName = nextStateName;
	m_animationSwitchDelayTimeInterval = delay;
	m_animationSwitchDelayTimeCnt = 0.0f;
	SetComponentTickEnabled(true);
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

