// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "SmithAnimationComponent.h"
#include "Debug.h"

// Sets default values for this component's properties
USmithAnimationComponent::USmithAnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmithAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	MontageToPlay = LoadObject<UAnimMontage>(nullptr, *objectPass);
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		USkeletalMeshComponent* skeltalmeshcomp = OwnerActor->FindComponentByClass<USkeletalMeshComponent>();
		if (skeltalmeshcomp)
		{
			AnimInstance = skeltalmeshcomp->GetAnimInstance();
		}
	}
	// �����^�[�W���I���̃C�x���g�o�^
	if (AnimInstance)
	{
		//AnimInstance->OnMontageEnded(this, OnMontageEnded);
	}

}


// Called every frame
void USmithAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USmithAnimationComponent::SwitchAnimState(FName m_Anim)
{
	// ���݂̃����^�[�W�����擾
	if (AnimInstance == nullptr)
	{
		return;
	}

	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	FName CurrentSection = AnimInstance->Montage_GetCurrentSection(CurrentMontage);

	// ���݂̃����^�[�W�����Đ������ǂ������m�F
	if (CurrentSection == m_Anim) { return; }
	// �V���������^�[�W�����Đ�
	AnimInstance->Montage_Play(MontageToPlay);
	AnimInstance->Montage_JumpToSection(m_Anim, MontageToPlay);
}

void USmithAnimationComponent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

}

