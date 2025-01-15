// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "SmithAIStrategyContainer.h"
#include "SmithAIBehaviorProcessor.generated.h"

DECLARE_DELEGATE_RetVal(bool, FTickCondition);

class USmithAIStrategy;

/**
 * 
 */
UCLASS()
class SMITH_API USmithAIBehaviorProcessor : public UObject , public FTickableGameObject
{
	GENERATED_BODY()

public:
	USmithAIBehaviorProcessor(const FObjectInitializer&);

	void BeginDestroy() override final;

	// start of FTickableObjectBase Interface
	#pragma region FTickableObjectBase Interface
		void Tick(float DeltaTime) override final;
		bool IsTickable() const override final;
		TStatId GetStatId() const override final;
	#pragma endregion
	// end of FTickableObjectBase Interface

	// start of FTickableObject Interface
	#pragma region FTickableObject Interface
		bool IsTickableWhenPaused() const override final;
		bool IsTickableInEditor() const override final;
		UWorld* GetTickableGameObjectWorld() const override final;
	#pragma endregion
	// end of FTickableObject Interface

public:
	void RegisterAIStrategy(int32 priority, USmithAIStrategy*);
	void EmptyStrategy();
	void RunBehaviorProcessor();
	void StopBehaviorProcessor();

public:
	FTickCondition TickConditionDelegate;
private:
	TArray<FSmithAIStrategyContainer> m_strategyContainer;
	uint8 m_bIsProcessorRunning : 1;
};
