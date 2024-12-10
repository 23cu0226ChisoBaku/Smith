// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPlayerCharacter.generated.h"

// �O���錾
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SMITH_API AMyPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �X�v�����O�A�[��
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_pSpringArm;
	// �J����
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_pCamera;

private:
	void UpdatePawn();
	void Pawn_MoveForward(float axisValue);
	void Pawn_MoveRight(float axisValue);

	FVector m_pos;
	FRotator m_dir;
};
