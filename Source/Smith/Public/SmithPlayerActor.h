// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ITurnManageable.h"
#include "SmithPlayerActor.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SMITH_API ASmithPlayerActor final: public APawn , public ITurnManageable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASmithPlayerActor();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

public:	
	// Called every frame
	void Tick(float DeltaTime) override final;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override final;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> m_springArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> m_cam;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTurnControlComponent> m_turnComponent;

	// Enhanced Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> m_mappingCtx;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_attackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_cameraAction;

public:
	UTurnControlComponent* GetTurnControl() const override final;
	FDelegateHandle Subscribe(FRequestCommandEvent::FDelegate&) override final;
	bool Unsubscribe(UObject*,FDelegateHandle) override final;

private:
	// input bind method
	void Move(const FInputActionValue& value);
	void Attack(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

private:
	FRequestCommandEvent m_event;
};
