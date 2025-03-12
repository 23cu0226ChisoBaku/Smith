// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "TestTypeErasure.generated.h"

UCLASS()
class SMITH_API ATestTypeErasure : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestTypeErasure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	float TimeInterval = 5.0f;

};
