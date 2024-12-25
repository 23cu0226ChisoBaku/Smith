// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/WeakInterfacePtr.h"
#include "ITurnManageable.h"
#include "ICanCommandMediate.h"
#include "TurnBaseActor.generated.h"

class IBattleCommand;
class IMoveable;
class ICanMakeAttack;
class IAttackable;
struct AttackHandle;

UCLASS()
class SMITH_API ATurnBaseActor : public AActor , public ITurnManageable , public ICanCommandMediate
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurnBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime);

public:

	// Interfaces
	#pragma region Interfaces

		// ITurnManageable
		#pragma region ITurnManageable
		UTurnControlComponent* GetTurnControl() const override final;
		FDelegateHandle Subscribe(FRequestCommandEvent::FDelegate&) override final;
		bool Unsubscribe(UObject*, FDelegateHandle) override final;
		#pragma endregion ITurnManageable
		// end of ITurnManageable

		// ICanCommandMediate
		#pragma region ICanCommandMediate
		void SetCommandMediator(ICommandMediator*) override final;
		#pragma endregion ICanCommandMediate
		// end of ICanCommandMediate

protected:
	void SendMoveCommand(IMoveable*);
	void SendAttackCommand(ICanMakeAttack*, IAttackable*, AttackHandle&&);

	#pragma endregion Interfaces
	// end of Interfaces
protected:
	UPROPERTY()
	TObjectPtr<UTurnControlComponent> TurnComponent;
private:
	TWeakInterfacePtr<ICommandMediator> m_commandMediator;
	
};
