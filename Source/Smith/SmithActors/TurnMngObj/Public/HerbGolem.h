// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBaseActor.h"
#include "IAttackable.h"
#include "ICanSetOnMap.h"
#include "IMoveDirector.h"

#include "ISmithAnimator.h"

#include "ISmithSimpleAIDriven.h"
#include "MapObjType.h"
#include "../Weapon/Params.h"
#include "ICanRequestEventPublishment.h"
#include "ISmithBattleLogger.h"

#include "IHealable.h"
// TODO
#include "SmithEnemyTraits.h"

#include "HerbGolem.generated.h"

struct FSmithAIStrategyContainer;
class USmithTurnBaseAIAttackStrategy;
class USmithTurnBaseAIMoveStrategy;
class USmithTurnBaseAIIdleStrategy;
class USmithTurnBaseAIHealStrategy;
class USmithAttackComponent;
class USmithMoveComponent;
class USmithMoveDirector;
class USmithPickable;

class USmithAnimationComponent;

// TODO
class USmithBattleLogWorldSubsystem;
/**
 * 
 */
UCLASS()
class SMITH_API AHerbGolem final:  public ATurnBaseActor, public IAttackable, 
																	  public ICanSetOnMap, public IMoveDirector, 
																		public ISmithSimpleAIDriven, public ICanRequestEventPublishment,
																		public ISmithBattleLogger,public IHealable,
																		public ISmithAnimator
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
	void OnAttack(AttackHandle&&) override final;

	uint8 GetOnMapSizeX() const override final;
	uint8 GetOnMapSizeY() const override final;
	EMapObjType GetType() const override final;
	void TurnOnAI() override final;

public:
	UClass* GetMoveDirectorUClass() const override final;
	void SetMoveDirector(USmithMoveDirector*) override final;
	uint8 GetChaseRadius() const override final;
	void SetEventPublishMediator(IEventPublishMediator*) override;

	void SwitchAnimation(uint8 animationState) override;
	void SwitchAnimationDelay(uint8 animationState, float delay) override;
	void UpdateAnimation(float deltaTime) override;
	bool IsAnimationFinish() const override;

	FString GetName_Log() const override;
	EBattleLogType GetType_Log() const override;

	void Heal() override final;
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
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIHealStrategy> m_healStrategy;
	UPROPERTY()
	TObjectPtr<USmithAttackComponent> m_atkComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USmithMoveComponent> MoveComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USmithAnimationComponent> AnimComponent;

		// Attack Format
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
	TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithAI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USmithMoveDirector> MoveDirectorSubclass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithAI, meta = (AllowPrivateAccess = "true"))
	uint8 ChaseRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapObjectType, meta = (AllowPrivateAccess = "true"))
	EMapObjType MapObjectType;
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
