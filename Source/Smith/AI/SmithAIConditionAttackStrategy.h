// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIAttackStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithSkillParameter.h"

#include "SmithAIConditionAttackStrategy.generated.h"

class ICommandMediator;
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
		FSmithSkillParameter SkillParameter;
	};
public:
	USmithAIConditionAttackStrategy(const FObjectInitializer&);
	virtual void BeginDestroy() override;
	// 初期化
	void Initialize(ICommandMediator*, int32 attackPower);
	void SetAttackParam(int32 attackPower, int32 critical, int32 level);
	// 条件の登録
	void ConditionResgister(const FString &name, const UDataTable *formatTable, const TDelegate<bool(void)>&, FSmithSkillParameter);

private:
	virtual bool executeImpl() override;
	
private:
	UPROPERTY()
	TObjectPtr<ASmithDangerZoneDisplayer> m_dangerZoneDisplayer; 
	TQueue<FConditionHandle> m_conditions;
	TWeakInterfacePtr<ICommandMediator> m_mediator;
	int32 m_atk;
	int32 m_crt;
	int32 m_level;
	int32 m_curtConditionCooldown;

	uint8 m_bIsDisplayingDangerZone : 1;
	uint8 m_bIsWaitCondition : 1;
};

