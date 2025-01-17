// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithTurnBaseAIMoveStrategy.generated.h"

class IMoveable;
class ICommandMediator;
/**
 * 
 */
UCLASS()
class SMITH_API USmithTurnBaseAIMoveStrategy : public USmithAIStrategy
{
	GENERATED_BODY()
	
public:
	USmithTurnBaseAIMoveStrategy(const FObjectInitializer&);

public:
	void Initialize(ICommandMediator* ,IMoveable*, uint8 moveSpeed);
	virtual void BeginDestroy() override;

private:
	virtual bool executeImpl() override;

private:
	TWeakInterfacePtr<ICommandMediator> m_mediator;
	TWeakInterfacePtr<IMoveable> m_move;
	uint8 m_moveSpeed;

};
