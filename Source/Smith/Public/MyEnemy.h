// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h" // �ǉ�
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"  // DrawDebugLine��DrawDebugPoint�Ȃǂ��g�p���邽�߂ɕK�v
#include "ITurnManageable.h"
#include "IAttackable.h"
#include "MyEnemy.generated.h"


class UTurnControlComponent;
class USmithMoveComponent;

UCLASS()
class SMITH_API AMyEnemy : public AActor , public ITurnManageable, public IAttackable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int32 HP;
	UPROPERTY(EditAnywhere)
	int32 ATK;

// TODO Create by Mai
#pragma region ITurnManageable Interface
public:
	UTurnControlComponent* GetTurnControl() const override final;
	FDelegateHandle Subscribe(FRequestCommandEvent::FDelegate&) override final;
	bool Unsubscribe(UObject*,FDelegateHandle) override final;

	void OnAttack(const AttackHandle&) override;
private:
	UPROPERTY()
	TObjectPtr<UTurnControlComponent> m_turnCtrl;

private:
	FRequestCommandEvent m_event;
#pragma endregion

private:
	void PlayerCheck();
	FVector MoveDirection();
	
	UPROPERTY()
	USmithMoveComponent* m_moveComp;
	UPROPERTY()
	UActorComponent* m_attackComp;

	AActor* m_target;

	float MOVE_DISTANCE;

	float m_timer;
};
