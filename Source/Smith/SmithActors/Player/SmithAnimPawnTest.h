// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ISmithAnimator.h"
#include "Smith/Public/SmithAnimationComponent.h"
#include "SmithAnimPawnTest.generated.h"

class USmithAnimationComponent;

UCLASS()
class SMITH_API ASmithAnimPawnTest : public APawn, public ISmithAnimator
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASmithAnimPawnTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SwitchAnimation(uint8 animationState) override;
	virtual void SwitchAnimationDelay(uint8,float) override;
	virtual void UpdateAnimation(float deltaTime) override;
	virtual bool IsAnimationFinish() const override;

	UFUNCTION(BlueprintNativeEvent, Category = "Events")
	void OpenGameOverUI();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithAnimationComponent>AnimationComp;

	UPROPERTY(EditAnywhere)
	int32 state;

};
