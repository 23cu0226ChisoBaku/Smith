// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h" // 追加
#include "InputAction.h" // 追加
#include "InputActionValue.h" // 追加
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
	// Pawnを更新
	void UpdatePawn();
	// 前の移動入力
	void Pawn_MoveForward(float axisValue);
	// 横の移動入力
	void Pawn_MoveRight(float axisValue);

	// Input設定
	void SetupInput();

	// Input Event(Action) イベントハンドラー関数
	void PressedAction();
	void ReleasedAction();

	// Input Event(Axis) イベントハンドラー関数
	void PressedAxis(const FInputActionValue& Value);

	// Press状態を管理
	bool IsPressed = false;
};
