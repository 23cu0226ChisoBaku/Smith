// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TurnBaseActor.h"
#include "IAttackable.h"
#include "IMoveDirector.h"

#include "ISmithAnimator.h"

#include "ISmithSimpleAIDriven.h"
#include "MapObjType.h"
#include "../Weapon/Params.h"
#include "ICanRequestEventPublishment.h"
#include "ISmithBattleLogger.h"
// TODO
#include "SmithEnemyTraits.h"

#include "HerbGolem.generated.h"

struct FSmithAIStrategyContainer;
class USmithTurnBaseAIAttackStrategy;
class USmithTurnBaseAIMoveStrategy;
class USmithTurnBaseAIIdleStrategy;

class USmithMoveDirector;
class USmithPickable;

class USmithAnimationComponent;

// TODO
class USmithBattleLogWorldSubsystem;
/**
 * 
 */
UCLASS()
class SMITH_API AHerbGolem final: public ATurnBaseActor, public IAttackable, 
																	public IMoveDirector,  public ISmithAnimator,
																	public ISmithSimpleAIDriven, public ICanRequestEventPublishment,
																	public ISmithBattleLogger
{
	GENERATED_BODY()

// Enemy Traits
public:
	using Type = HerbGolem;

public:
	AHerbGolem();

protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;
	
public:
	void Tick(float DeltaTime) override final;

public:
	void OnAttack(const AttackHandle&) override final;
	bool IsDefeated() const override final
	{
		return true;
	}

	void OnDefeated() override final
	{
		
	}

	void TurnOnAI() override final;

public:
	UClass* GetMoveDirectorUClass() const override final;
	void SetMoveDirector(USmithMoveDirector*) override final;
	uint8 GetChaseRadius() const override final;
	void SetEventPublishMediator(IEventPublishMediator*) override;

	void SwitchAnimation(uint8 animationState) override;
	void UpdateAnimation(float deltaTime) override;
	bool IsAnimationFinish() const override;

	FString GetName_Log() const override;
	EBattleLogType GetType_Log() const override;

	bool HealCondition();

public:
	void InitializeParameter(int32 currentLevel) override final;

private:
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIAttackStrategy> m_attackStrategy;
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIMoveStrategy> m_moveStrategy;
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIIdleStrategy> m_idleStrategy;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USmithAnimationComponent> AnimComponent;

		// Attack Format
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
	TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithAI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USmithMoveDirector> MoveDirectorSubclass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithAI, meta = (AllowPrivateAccess = "true"))
	uint8 ChaseRadius;
	UPROPERTY()
	TObjectPtr<USmithMoveDirector> m_moveDirector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = DropItemTable, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USmithPickable>> DropUpgradeTable; 

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BattleParameter, meta = (AllowPrivateAccess = "true"))
	FParams EnemyParam;

	TWeakInterfacePtr<IEventPublishMediator> m_eventMediator;

	// TODO
	UPROPERTY()
	TObjectPtr<USmithBattleLogWorldSubsystem> m_logSystem;
	int32 m_maxHp;
	int32 m_healCnt;
};
