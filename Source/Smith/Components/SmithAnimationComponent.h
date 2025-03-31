// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"

#include "SmithAnimationComponent.generated.h"

class UAnimMontage;
class UAnimInstance;

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

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimMontage> MontageToPlay;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAnimInstance> AnimInstance;

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
