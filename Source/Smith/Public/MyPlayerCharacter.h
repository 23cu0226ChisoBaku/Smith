// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPlayerCharacter.generated.h"

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
private:
	// Pawn‚ğXV
	void UpdatePawn();
private:
	// ‘O‚ÌˆÚ“®“ü—Í
	void Pawn_MoveForward(float axisValue);
	// ‰¡‚ÌˆÚ“®“ü—Í
	void Pawn_MoveRight(float axisValue);

private:
	FVector2f m_pawnMoveInput;
};
