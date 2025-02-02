// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SmithGameOver/ICallable.h"
#include "SmithGameOver/GameOver_Observer.h"
#include "GameOver_TestPawn.generated.h"

UCLASS()
class SMITH_API AGameOver_TestPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGameOver_TestPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDead();

	UPROPERTY(EditAnywhere)
	bool gameover;

	UPROPERTY(EditAnywhere)
	AGameOver_Observer* GameOverActor;

};
