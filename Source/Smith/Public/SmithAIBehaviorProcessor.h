// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SmithAIStrategyContainer.h"
#include "SmithAIBehaviorProcessor.generated.h"

DECLARE_DELEGATE_RetVal(bool, FTickCondition);

class USmithAIStrategy;

/**
 * 
 */
UCLASS()
class SMITH_API USmithAIBehaviorProcessor : public UObject
{
	GENERATED_BODY()

public:
	USmithAIBehaviorProcessor(const FObjectInitializer&);

	void BeginDestroy() override final;

public:
	void RegisterAIStrategy(int32 priority, USmithAIStrategy*);
	void EmptyStrategy();
	void RunBehaviorProcessor();
	void TickBehaviorProcessor(float deltaTime);
	void StopBehaviorProcessor();

public:
	FTickCondition TickConditionDelegate;
private:
	TArray<FSmithAIStrategyContainer> m_strategyContainer;
	uint8 m_bIsProcessorRunning : 1;
};
