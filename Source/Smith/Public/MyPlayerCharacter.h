// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h" // 追加
#include "InputAction.h"		 // 追加
#include "InputActionValue.h"	 // 追加
#include "MyPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SMITH_API AMyPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	/** Called for Action input */
	void EventAction(const FInputActionValue &Value);

	/** Called for Axis input */
	void EventAxis(const FInputActionValue &Value);

	/** MappingContext */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputMappingContext *DefaultMappingContext;

	/** Action Input */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *ActionInput;

	/** Axis Input */
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction *AxisInput;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere)
	float MOVE_DISTANCE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *m_pSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent *m_pCamera;

private:
	void UpdatePawn();
	void Pawn_MoveForward();
	void Pawn_MoveBackward();
	void Pawn_MoveRight();
	void Pawn_MoveLeft();

	// Input設定
	void SetupInput();

	// Input Event(Action) イベントハンドラー関数
	void PressedAction();
	void ReleasedAction();

	// Input Event(Axis) イベントハンドラー関数
	void PressedAxis(const FInputActionValue &Value);

	FVector m_pos;
	FRotator m_dir;
	float moveTimer;
	bool isMove;
	// Press状態を管理
	bool IsPressed = false;
};
