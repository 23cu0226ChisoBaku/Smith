// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithAIAttackStrategy.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithTurnBaseAIAttackStrategy.generated.h"

namespace UE::Smith
{
	namespace Battle
	{
		enum class EMoveDirection : uint8;
	}
}
class ICommandMediator;
class ICanMakeAttack;

DECLARE_DELEGATE_OneParam(FOnChangeFaceDirectionEvent, UE::Smith::Battle::EMoveDirection);

/**
 * 
 */
UCLASS()
class SMITH_API USmithTurnBaseAIAttackStrategy final: public USmithAIAttackStrategy
{
	GENERATED_BODY()	
	
	public:
		USmithTurnBaseAIAttackStrategy(const FObjectInitializer&);
		void Initialize(ICanMakeAttack*, ICommandMediator*);
		void BeginDestroy() override final;

	private:
		bool executeImpl() override final;

	public:
		FOnChangeFaceDirectionEvent OnChangeDirectionDelegate;

	private:
		TWeakInterfacePtr<ICommandMediator> m_mediator;
		TWeakInterfacePtr<ICanMakeAttack> m_attacker;

};
