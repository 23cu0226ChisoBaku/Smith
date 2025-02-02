// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithActors/TurnMngObj/Public/SmithEnemy.h"
#include "ICanSetOnMap.h"
#include "ISmithSimpleAIDriven.h"
#include "MapObjType.h"
#include "../Weapon/Params.h"
#include "ICanRequestEventPublishment.h"
#include "SmithBoss.generated.h"

class USmithAIConditionAttackStrategy;
class USmithTurnBaseAIMoveStrategy;
class USmithTurnBaseAIIdleStrategy;
class USmithAttackComponent;
class USmithPickable;

struct FSmithAIStrategyContainer;
struct FSmithAIConditionBindHandle;

DECLARE_DELEGATE_RetVal(bool,FConditionDelegate);

/**
 *
 */
UCLASS()
class SMITH_API ASmithBoss final : public ASmithEnemy,
																	 public ICanSetOnMap,
																	 public ISmithSimpleAIDriven,
																	 public ICanRequestEventPublishment
{
	GENERATED_BODY()

public:
	ASmithBoss();

protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

public:
	void Tick(float DeltaTime) override final;

public:
	void OnAttack(AttackHandle &&) override final;

	uint8 GetOnMapSizeX() const override final;
	uint8 GetOnMapSizeY() const override final;
	EMapObjType GetType() const override final;
	void TurnOnAI() override final;

// その技を使う条件の関数
private:
	bool RageCondition();
	UFUNCTION()
	bool WingsCondition();
	UFUNCTION()
	bool BreathCondition();
	UFUNCTION()
	bool SweepCondition();
	bool NormalCondition();

public:
	void SetEventPublishMediator(IEventPublishMediator *) override;

private:
	UPROPERTY()
	TObjectPtr<USmithAIConditionAttackStrategy> m_attackStrategy;
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIMoveStrategy> m_moveStrategy;
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIIdleStrategy> m_idleStrategy;
	UPROPERTY()
	TObjectPtr<USmithAttackComponent> m_atkComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USmithMoveComponent> MoveComponent;

	// Attack Format
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
	TMap<FString, FSmithAIConditionBindHandle> ConditionAttackFormatTables;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapObjectType, meta = (AllowPrivateAccess = "true"))
	EMapObjType MapObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = DropItemTable, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USmithPickable>> DropUpgradeTable;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BattleParameter, meta = (AllowPrivateAccess = "true"))
	FParams EnemyParam;

	TWeakInterfacePtr<IEventPublishMediator> m_eventMediator;

private:
  FConditionDelegate m_conditionCallBack;
	int32 m_maxHp;
	int32 m_wingsCnt;
	int32 m_breathCnt;
	int32 m_sweepCnt;
	bool m_isRage;
};
