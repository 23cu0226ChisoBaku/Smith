// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIAttackStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithTurnBaseAIAttackStrategy.generated.h"

class ICommandMediator;
class ICanMakeAttack;
enum class EDirection : uint8;

DECLARE_DELEGATE_OneParam(FOnChangeFaceDirectionEvent, EDirection);

/**
 * 
 */
UCLASS()
class SMITH_API USmithTurnBaseAIAttackStrategy final: public USmithAIAttackStrategy
{
	GENERATED_BODY()	
	
public:
	USmithTurnBaseAIAttackStrategy(const FObjectInitializer&);
	void Initialize(ICanMakeAttack*, ICommandMediator*, int32 attackPower);
	void BeginDestroy() override final;

private:
	bool executeImpl() override final;

public:
	FOnChangeFaceDirectionEvent OnChangeDirectionDelegate;

private:
	TWeakInterfacePtr<ICommandMediator> m_mediator;
	TWeakInterfacePtr<ICanMakeAttack> m_attacker;
	int32 m_atk;
};
