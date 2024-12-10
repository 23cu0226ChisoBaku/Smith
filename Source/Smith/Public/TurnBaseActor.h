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
	UTurnControlComponent* GetTurnControl() const override;
	FDelegateHandle Subscribe(FRequestCommandEvent::FDelegate&) override;
	bool Unsubscribe(UObject*, FDelegateHandle) override;

protected:
	void SendCommand(IBattleCommand*);

private:
	UPROPERTY()
	TObjectPtr<UTurnControlComponent> m_turnComponent;
private:
	FRequestCommandEvent m_event;
	
};
