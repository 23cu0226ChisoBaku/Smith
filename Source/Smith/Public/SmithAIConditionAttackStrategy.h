// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIAttackStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithSkillCenterSpotParameter.h"
#include "SmithAIConditionAttackStrategy.generated.h"

class ICommandMediator;
class ICanMakeAttack;
class ASmithDangerZoneDisplayer;

/**
 *
 */
UCLASS()
class SMITH_API USmithAIConditionAttackStrategy : public USmithAIAttackStrategy
{
	GENERATED_BODY()

	struct FConditionHandle
	{
		FString Name;
		TDelegate<bool(void)> Condition;
		FSmithSkillCenterSpotParameter SkillParameter;
	};
public:
	USmithAIConditionAttackStrategy(const FObjectInitializer&);
	// 初期化
	void Initialize(ICanMakeAttack*, ICommandMediator*, int32 attackPower);
	// 条件の登録
	void ConditionResgister(const FString &name, const UDataTable *formatTable, const TDelegate<bool(void)>&, FSmithSkillCenterSpotParameter);

private:
	virtual bool executeImpl() override;
	
private:
	UPROPERTY()
	TObjectPtr<ASmithDangerZoneDisplayer> m_dangerZoneDisplayer; 
	TQueue<FConditionHandle> m_conditions;
	TWeakInterfacePtr<ICommandMediator> m_mediator;
	TWeakInterfacePtr<ICanMakeAttack> m_attacker;
	int32 m_atk;

	uint8 m_bIsDisplayingDangerZone : 1;
};

