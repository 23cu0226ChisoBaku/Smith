// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TurnBaseActor.h"
#include "IAttackable.h"

#include "SmithEnemy.generated.h"

/**
 *
 */
UCLASS()
class SMITH_API ASmithEnemy : public ATurnBaseActor, public IAttackable
{
	GENERATED_BODY()
	
public:
	ASmithEnemy();

public:
	void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnAttack(const FBattleResult& Handle) override;
	virtual bool IsDefeated() const override
	{
		return m_hp <= 0;
	}
	virtual void OnDefeated() override
	{ }
	
	void OnHeal(int32);

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateHp();

public:
	UPROPERTY(BluePrintReadOnly)
	int32 Max_HP;
	UPROPERTY(BluePrintReadOnly)
	int32 Current_HP;

protected:
	// 数を整数値にする//
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
