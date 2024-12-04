// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestTileMove.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

private:
	bool moveTile_test();
	bool attack_test();


private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* m_springArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_cam;

private:
	FVector m_InputDir;
	bool m_isInAction;
};
