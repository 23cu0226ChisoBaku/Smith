// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h" // �ǉ�
#include "InputAction.h" // �ǉ�
#include "InputActionValue.h" // �ǉ�
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

	/** Called for Action input */
	void EventAction(const FInputActionValue& Value);

	/** Called for Axis input */
	void EventAxis(const FInputActionValue& Value);

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext* DefaultMappingContext;

	/** Action Input */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ActionInput;

	/** Axis Input */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* AxisInput;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Pawn���X�V
	void UpdatePawn();
	// �O�̈ړ�����
	void Pawn_MoveForward(float axisValue);
	// ���̈ړ�����
	void Pawn_MoveRight(float axisValue);

	// Input�ݒ�
	void SetupInput();

	// Input Event(Action) �C�x���g�n���h���[�֐�
	void PressedAction();
	void ReleasedAction();

	// Input Event(Axis) �C�x���g�n���h���[�֐�
	void PressedAxis(const FInputActionValue& Value);

	// Press��Ԃ��Ǘ�
	bool IsPressed = false;
};
