// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TurnBaseActor.h"

#include "IAttackable.h"
#include "IMoveDirector.h"
#include "ISmithAnimator.h"
#include "ISmithSimpleAIDriven.h"
#include "MapObjType.h"
#include "Params.h"
#include "ICanRequestEventPublishment.h"
#include "SmithEnemyTraits.h"

#include "SmallGolem.generated.h"

struct FSmithAIStrategyContainer;
class USmithTurnBaseAIAttackStrategy;
class USmithTurnBaseAIMoveStrategy;
class USmithTurnBaseAIIdleStrategy;
class USmithMoveDirector;
class USmithPickable;

class USmithAnimationComponent;

enum class EDirection : uint8;

/**
 * 
 */
UCLASS()
class SMITH_API ASmallGolem final: 	public ATurnBaseActor, public IAttackable, 
																				public IMoveDirector, public ISmithSimpleAIDriven, 
																				public ICanRequestEventPublishment, public ISmithAnimator
{
	GENERATED_BODY()

// Enemy Traits
public:
	using Type = SmallGolem;

public:
	ASmallGolem();

protected:
	void BeginPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;
	
public:
	void Tick(float DeltaTime) override final;

public:
	//---Begin of IAttackable Interface
	void OnAttack(const FBattleResult& Handle) override final;
	bool IsDefeated() const override final;
	void OnDefeated() override final;

	void TurnOnAI() override final;

public:
	UClass* GetMoveDirectorUClass() const override final;
	void SetMoveDirector(USmithMoveDirector*) override final;
	uint8 GetChaseRadius() const override final;
	void SetEventPublishMediator(IEventPublishMediator*) override;

	void SwitchAnimation(uint8 animationState) override;
	void UpdateAnimation(float deltaTime) override;
	bool IsAnimationFinish() const override;

	FBattleDefenseParamHandle GetDefenseParam() const override;
	

public:
	void InitializeParameter(int32 currentLevel) override final;

private:
	void faceToDirection(EDirection);
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
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BattleParameter, meta = (AllowPrivateAccess = "true"))
	FParams EnemyParam;
	
private:
	TWeakInterfacePtr<IEventPublishMediator> m_eventMediator;
	int32 m_level;
	uint8 m_bIsPlayingDeadAnimation : 1;
	uint8 m_bIsPlayingDamagedAnimation : 1;
};
