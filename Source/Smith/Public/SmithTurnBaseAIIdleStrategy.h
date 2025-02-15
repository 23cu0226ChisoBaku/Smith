// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithTurnBaseAIIdleStrategy.generated.h"

class ICommandMediator;

/**
 * 
 */
UCLASS()
class SMITH_API USmithTurnBaseAIIdleStrategy : public USmithAIStrategy
{
	GENERATED_BODY()

public:
	USmithTurnBaseAIIdleStrategy(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	void Initialize(ICommandMediator*, float idleTime = 0.0f);

private:
	bool executeImpl() override final;

private: 
	TWeakInterfacePtr<ICommandMediator> m_mediator;
	float m_idleTime;
};
