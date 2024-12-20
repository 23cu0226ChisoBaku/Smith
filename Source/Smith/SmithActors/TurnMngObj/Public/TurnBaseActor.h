// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ITurnManageable.h"
#include "TurnBaseActor.generated.h"

class IBattleCommand;

UCLASS()
class SMITH_API ATurnBaseActor : public AActor , public ITurnManageable
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
	UTurnControlComponent* GetTurnControl() const override final;
	FDelegateHandle Subscribe(FRequestCommandEvent::FDelegate&) override final;
	bool Unsubscribe(UObject*, FDelegateHandle) override final;

protected:
	void SendCommand(TSharedPtr<IBattleCommand>);

protected:
	UPROPERTY()
	TObjectPtr<UTurnControlComponent> TurnComponent;
private:
	FRequestCommandEvent m_event;
	
};
