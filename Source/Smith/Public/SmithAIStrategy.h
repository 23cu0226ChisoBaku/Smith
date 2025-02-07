// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SmithAIStrategy.generated.h"

/**
 * 
 */


UCLASS(Abstract,EditInlineNew)
class SMITH_API USmithAIStrategy : public UObject
{
	GENERATED_BODY()

public:
	USmithAIStrategy(const FObjectInitializer&);

public:
	virtual void BeginDestroy() override;
	bool Execute();
	void SetOwner(AActor*);
	AActor* GetOwner() const;

private:
	virtual bool executeImpl() PURE_VIRTUAL(USmithAIStrategy::executeImpl, return false;);

private:
	UPROPERTY()
	TObjectPtr<AActor> m_owner;
};
