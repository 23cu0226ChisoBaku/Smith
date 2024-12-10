// Fill out your copyright notice in the Description page of Project Settings.
/*

TestTileMove.h

Author : MAI ZHICONG

Description : プロトタイププレイヤー移動クラス

Update History: 2024/12/04 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "IAttackable.h"
#include "ITurnManageable.h"
#include "TestTileMove.generated.h"

class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SMITH_API ATestTileMove : public APawn, public IAttackable
{
	GENERATED_BODY()

private:
	enum EDir_Test : uint8
	{
		North = 0,						// 上方向
		NorthEast = 1,				// 右上
		East = 2,							// 右
		SouthEast = 3,				// 右下
		South = 4,						// 下
		SouthWest = 5,				// 左下
		West = 6,							// 左
		NorthWest = 7,				// 左上

		DirectionCnt,					// 選べられる方向の数
	};

//---------------------------------------
/*
                  ctor
*/
//---------------------------------------
public:
	// Sets default values for this pawn's properties
	ATestTileMove();

//---------------------------------------
/*
           アクター ライフサイクル
*/
//---------------------------------------
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//---------------------------------------
/*
       パブリック関数(インターフェース)
*/
//---------------------------------------

public:
// IAttackable(UInterface)
#pragma region IAttackable
	void OnAttack(const AttackHandle& attack) override;
#pragma endregion

//---------------------------------------
/*
              ufunction 宣言
*/
//---------------------------------------
public:
	UFUNCTION(BlueprintCallable, Category = "TestTileMove_Func")
	void SetCameraPos(const FVector& pos);
	UFUNCTION(BlueprintCallable, Category = "TestTileMove_Func")
	void SetCameraAngle(const FRotator& rotate);

//---------------------------------------
/*
              uproperty 宣言
*/
//---------------------------------------
private:
	// Subobjects
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> m_capsuleCol;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> m_springArm;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> m_cam;

	// Enhanced Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> m_mappingCtx;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_attackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> m_cameraAction;

	// Blueprint Edit
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector CamPos;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator CamAngle;


private:
	// class method
	bool moveTile_test();
	bool attack_test();
	bool updateCam_test();
	bool changeForward_test(const FVector2D& inputValue);
	void setupInputMappingCtx_test();

private:
	// TODO
	EDir_Test vectorToEDir(const FVector& direction);

private:
	// input bind method
	void Move(const FInputActionValue& value);
	void Attack(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
//---------------------------------------
/*
            プライベートプロパティ
*/
//---------------------------------------
#pragma region Private Properties
private:
	FVector m_nextDir;
	EDir_Test m_camDir;
	EDir_Test m_actorFaceDir;
	float m_capsuleHalfHeight;
	uint8 m_hasMoveInput : 1;
	uint8 m_hasAttackInput : 1;
	uint8 m_hasLookInput :1;
	uint8 m_isInAction : 1;
#pragma endregion
	
};
