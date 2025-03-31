// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmithEnemy.h"
#include "ISmithAnimator.h"
#include "ISmithSimpleAIDriven.h"
#include "Params.h"
#include "ICanRequestEventPublishment.h"
#include "SmithAIConditionBindHandle.h"
#include "SmithEnemyTraits.h"
#include "IAttackCauser.h"

#include "SmithBoss.generated.h"

class USmithAIConditionAttackStrategy;
class USmithTurnBaseAIMoveStrategy;
class USmithTurnBaseAIIdleStrategy;
class USmithBattleLogWorldSubsystem;
class USmithAnimationComponent;
struct FSmithAIStrategyContainer;


DECLARE_DELEGATE_RetVal(bool, FConditionDelegate);

/**
 *
 */
UCLASS()
class SMITH_API ASmithBoss final: public ASmithEnemy, public ISmithSimpleAIDriven,
                                  public ICanRequestEventPublishment, public ISmithAnimator, 
                                  public IAttackCauser
{
  GENERATED_BODY()

public:
  using Type = typename Dragon;

public:

  ASmithBoss();

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

  void SwitchAnimation(uint8 animationState) override;
  void UpdateAnimation(float deltaTime) override;
  bool IsAnimationFinish() const override;

  void InitializeParameter(int32 currentLevel) override final;

  void OnAttackExecuted() override
  {
  }

// その技を使う条件の関数
private:
  bool RageCondition();
  UFUNCTION()
  bool WingsCondition();
  UFUNCTION()
  bool BreathCondition();
  UFUNCTION()
  bool PressCondition();
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

  UPROPERTY(EditAnywhere)
  TObjectPtr<USmithAnimationComponent> AnimComponent;

  // Attack Format
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AttackFormat, meta = (AllowPrivateAccess = "true"))
  TMap<FString, FSmithAIConditionBindHandle> ConditionAttackFormatTables;

private:

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BattleParameter, meta = (AllowPrivateAccess = "true"))
  FParams EnemyParam;

  TWeakInterfacePtr<IEventPublishMediator> m_eventMediator;

private:
  int32 m_maxHp;
  int32 m_level;
  int32 m_wingsCnt;
  int32 m_breathCnt;
  int32 m_sweepCnt;
  bool m_isRage;
};
