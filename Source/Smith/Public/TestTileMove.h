// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestTileMove.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SMITH_API ATestTileMove : public APawn
{
	GENERATED_BODY()

private:
	enum EPlayerMoveDir_test : uint8
	{
		None = 0,							// 入力なし

		North = 1,						// 上方向
		NorthEast = 2,				// 右上
		East = 3,							// 右
		SouthEast = 4,				// 右下
		South = 5,						// 下
		SouthWest = 6,				// 左下
		West = 7,							// 左
		NorthWest = 8,				// 左上
	};

public:
	// Sets default values for this pawn's properties
	ATestTileMove();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION(BlueprintCallable, Category = "TestTileMove_Func")
	void SetCameraPos(const FVector& pos);
	UFUNCTION(BlueprintCallable, Category = "TestTileMove_Func")
	void SetCameraAngle(const FRotator& rotate);

private:
	bool moveTile_test();
	bool attack_test();
	void updateCam_test();

private:
	void Move(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> m_capsuleCol;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> m_springArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> m_cam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> m_mappingCtx;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_attackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector CamPos;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator CamAngle;

private:
	FVector m_InputDir;
	uint8 m_hasMoveInput : 1;
	uint8 m_hasAttackInput : 1;
	uint8 m_isInAction : 1;
	
};
