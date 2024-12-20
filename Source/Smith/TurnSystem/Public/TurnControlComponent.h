// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnPriority.h"
#include "TurnControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SMITH_API UTurnControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTurnControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void SetCommandSendable(bool);
	void SetTurnPriority(ETurnPriority);

	bool IsCommandSendable() const;
	ETurnPriority GetPriority() const;

private:
	ETurnPriority m_priority;
	uint8 m_isCMDSendable : 1;
		
};
