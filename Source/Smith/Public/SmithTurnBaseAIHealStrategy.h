// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithTurnBaseAIHealStrategy.generated.h"

class ICommandMediator;
class IHealable;
/**
 *
 */
UCLASS()
class SMITH_API USmithTurnBaseAIHealStrategy : public USmithAIStrategy
{
	GENERATED_BODY()

public:
USmithTurnBaseAIHealStrategy(const FObjectInitializer&);

public:
	void Initialize(ICommandMediator*,IHealable*);
	virtual void BeginDestroy() override;
	void RegisterCondition(const TDelegate<bool(void)>&);

private:
	virtual bool executeImpl() override;

private:
	TWeakInterfacePtr<ICommandMediator> m_mediator;
	IHealable* m_healable;
	TDelegate<bool(void)> m_condition;
};
