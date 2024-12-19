// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ITurnManageable.h"
#include "IAttackable.h"
#include "SmithPlayerActor.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USmithMoveComponent;
class USmithAttackComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class IBattleCommand;
struct AttackHandle;


UCLASS()
class SMITH_API ASmithPlayerActor final: public APawn , public ITurnManageable , public IAttackable
{
	GENERATED_BODY()

public:
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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithMoveComponent> m_moveComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USmithAttackComponent> m_atkComponent;

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

public:
	void OnAttack(AttackHandle&&) override final;

private:
	void sendCommand(TSharedPtr<IBattleCommand>);
	void moveImpl(FVector2D);
	void attackImpl();
	void changeFwdImpl(EDir_Test);

	// TODO
	bool searchActorsInDirection(FVector, TArray<AActor*>&);

private:
	// input bind method
	void Move_Input(const FInputActionValue& value);
	void Attack_Input(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

private:
	FRequestCommandEvent m_event;
	int32 m_hp;

	EDir_Test m_camDir;
	EDir_Test m_actorFaceDir;
	
	uint8 m_bCanMove : 1;
	uint8 m_bCanAttack : 1;
};