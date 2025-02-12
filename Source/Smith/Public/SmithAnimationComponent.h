// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "SmithAnimationComponent.generated.h"


class USkeltalMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMITH_API USmithAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USmithAnimationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SwitchAnimState(FName);
	void SwitchAnimStateDelay(FName, float delay);
	void UpdateAnim(float);
	bool IsCurrentAnimationFinish() const;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* MontageToPlay;
	UPROPERTY(VisibleAnywhere)
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere)
	FString objectPass;

	// TODO update
	float m_curtAnimationTimeInterval;
	float m_animationPlayTimeCnt;
	// TODO
	float m_animationSwitchDelayTimeInterval;
	float m_animationSwitchDelayTimeCnt;
	FName m_delayNextSectionName;
		
};
