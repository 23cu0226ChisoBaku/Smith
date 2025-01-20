// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TurnBaseActor.h"
#include "IAttackable.h"
#include "ICanSetOnMap.h"
#include "TurnActor_Test.generated.h"

struct FSmithAIStrategyContainer;
class USmithTurnBaseAIAttackStrategy;
class USmithTurnBaseAIMoveStrategy;
class USmithAttackComponent;
class USmithMoveComponent;
/**
 * 
 */
UCLASS()
class SMITH_API ATurnActor_Test final: public ATurnBaseActor, public IAttackable
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

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SmithAI, meta = (AllowPrivateAccess = "true"))
	TArray<FSmithAIStrategyContainer> AIRegistrationList;
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIAttackStrategy> m_attackStrategy;
	UPROPERTY()
	TObjectPtr<USmithTurnBaseAIMoveStrategy> m_moveStrategy;
	UPROPERTY()
	TObjectPtr<USmithAttackComponent> m_atkComponent;
	UPROPERTY()
	TObjectPtr<USmithMoveComponent> m_moveComponent;

		// Attack Format
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
	TMap<FString,TSoftObjectPtr<UDataTable>> AttackFormatTables;
};
