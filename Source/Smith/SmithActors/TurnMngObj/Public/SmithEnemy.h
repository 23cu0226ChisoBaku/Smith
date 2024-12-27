// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithActors/TurnMngObj/Public/TurnBaseActor.h"
#include "IAttackable.h"
#include "SmithEnemy.generated.h"

class USmithMoveComponent;
class USmithAttackComponent;

/**
 *
 */
UCLASS()
class SMITH_API ASmithEnemy : public ATurnBaseActor, public IAttackable
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASmithEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnAttack(AttackHandle &&) override;

private:
	UPROPERTY()
	TObjectPtr<USmithMoveComponent> m_moveComp;
	UPROPERTY()
	TObjectPtr<USmithAttackComponent> m_attackComp;

private:
	void PlayerCheck();
	FVector MoveDirection();

	AActor *m_target;
	float MOVE_DISTANCE;
	float m_timer;

	// TODO
	float m_hp;
};
