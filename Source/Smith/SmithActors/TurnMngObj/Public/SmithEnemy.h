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
	ASmithEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnAttack(AttackHandle &&) override;
	void OnHeal(int32);

protected:
	UPROPERTY()
	TObjectPtr<USmithMoveComponent> m_moveComp;
	UPROPERTY()
	TObjectPtr<USmithAttackComponent> m_attackComp;

protected:
	// マップが実装されたら引数を整数値にする
	AActor *PlayerCheck(float checkLenth = 1.0f);
	int32 m_skillCoolTurn;
	int32 m_skillCnt;
	int32 m_atk;
	int32 m_hp;

	float m_timer;

	uint8 MoveDirection();

private:
	// マップができて、経路探索ができたら消す
	AActor *m_target;
	float MOVE_DISTANCE;
};
