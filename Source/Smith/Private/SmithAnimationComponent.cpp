// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithAnimationComponent.h"

#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "AnimMontageHelperLibrary.h"

#include "Debug.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithAnimationComponent)

USmithAnimationComponent::USmithAnimationComponent()
	: m_curtAnimationTimeInterval(0.0f)
	, m_animationPlayTimeCnt(0.0f)
	, m_animationSwitchDelayTimeInterval(0.0f)
	, m_animationSwitchDelayTimeCnt(0.0f)
	, m_delayNextSectionName()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

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
	if (nextStateName.IsNone())
	{
		return;
	}

	if (AnimInstance == nullptr)
	{
		return;
	}

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage == nullptr)
	{
		m_curtAnimationTimeInterval = 0.0f;
		m_animationPlayTimeCnt = 0.0f;
		AnimInstance->Montage_JumpToSection(nextStateName);
		return;
	}

	FName CurrentSectionName = AnimInstance->Montage_GetCurrentSection(CurrentMontage);
	if (CurrentSectionName == nextStateName) 
	{ 
		return; 
	}

	const float duration = UAnimMontageHelperLibrary::GetSectionDuration(AnimInstance, CurrentMontage, nextStateName);
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

	AnimInstance->Montage_JumpToSection(nextStateName);

	//MDebug::LogWarning(FString::Printf(TEXT("[%s] Switch To AnimState [%s]. Duration : [%f]"), *GetNameSafe(GetOwner()), *nextStateName.ToString(), duration));
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


