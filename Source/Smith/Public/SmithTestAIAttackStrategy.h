// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIAttackStrategy.h"
#include "SmithTestAIAttackStrategy.generated.h"

/**
 * 
 */
UCLASS()
class SMITH_API USmithTestAIAttackStrategy : public USmithAIAttackStrategy
{
	GENERATED_BODY()

public:
	USmithTestAIAttackStrategy(const FObjectInitializer&);

	void BeginDestroy() override final;
	
private:
	bool executeImpl() override final;
};
