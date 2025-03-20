// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithTurnBaseAIMoveStrategy.generated.h"

class IMoveable;
class ICommandMediator;
class USmithMoveDirector;
enum class EDirection : uint8;
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
	void Initialize(ICommandMediator*, USmithMoveDirector* ,IMoveable*, uint8 moveSpeed);
	virtual void BeginDestroy() override;

private:
	virtual bool executeImpl() override;

public:
	TDelegate<void(EDirection)> OnMoveToEvent;
private:
	TWeakInterfacePtr<ICommandMediator> m_mediator;
	TWeakObjectPtr<USmithMoveDirector> m_moveDirector;
	TWeakInterfacePtr<IMoveable> m_move;
	uint8 m_moveSpeed;

};
