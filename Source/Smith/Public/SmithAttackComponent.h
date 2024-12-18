// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmithAttackComponent.generated.h"

class AMyPlayerCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SMITH_API USmithAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmithAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void Attack(AMyPlayerCharacter *player,int32 damage);

};
