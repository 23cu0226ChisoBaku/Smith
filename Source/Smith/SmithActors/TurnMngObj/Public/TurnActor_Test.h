// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBaseActor.h"
#include "IAttackable.h"
#include "ICanSetOnMap.h"
#include "IMoveDirector.h"
#include "ISmithSimpleAIDriven.h"
#include "MapObjType.h"
#include "../Weapon/Params.h"
#include "ICanRequestEventPublishment.h"
#include "TurnActor_Test.generated.h"

struct FSmithAIStrategyContainer;
class USmithTurnBaseAIAttackStrategy;
class USmithTurnBaseAIMoveStrategy;
class USmithTurnBaseAIIdleStrategy;
class USmithAttackComponent;
class USmithMoveComponent;
class USmithMoveDirector;
class USmithPickable;
/**
 * 
 */
UCLASS()
class SMITH_API ATurnActor_Test final: 	public ATurnBaseActor, public IAttackable, 
																				public ICanSetOnMap, public IMoveDirector, 
																				public ISmithSimpleAIDriven, public ICanRequestEventPublishment
{
	GENERATED_BODY()

public:
	ATurnActor_Test();

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

private:
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIAttackStrategy> m_attackStrategy;
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
	TMap<FString,TObjectPtr<USmithPickable>> DropUpgradeTable; 

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BattleParameter, meta = (AllowPrivateAccess = "true"))
	FParams EnemyParam;

	TWeakInterfacePtr<IEventPublishMediator> m_eventMediator;
};
