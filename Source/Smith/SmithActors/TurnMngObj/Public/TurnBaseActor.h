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
class USmithAIStrategy;
class USmithAIBehaviorProcessor;
struct AttackHandle;
enum class EDirection : uint8;

namespace UE::Smith
{
	namespace Battle
	{
		class FSmithCommandFormat;
	}
}

UCLASS(Abstract)
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
		// ICanCommandMediate
		#pragma region ICanCommandMediate
		void SetCommandMediator(ICommandMediator*) override final;
		#pragma endregion ICanCommandMediate
		// end of ICanCommandMediate

protected:
	void SendMoveCommand(IMoveable*, EDirection, uint8 moveDistance);
	void SendAttackCommand(ICanMakeAttack*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, AttackHandle&&);

	#pragma endregion Interfaces
	// end of Interfaces

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SmithAI, meta = (AllowPrivateAccess = "true"))
	bool bUseSmithAIProcessor;

protected:
	UPROPERTY()
	TObjectPtr<USmithAIBehaviorProcessor> m_aiBehaviorProcessor;

protected:
	TWeakInterfacePtr<ICommandMediator> m_commandMediator;
	
};
