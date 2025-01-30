// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithGameOver/GameOver_TestPawn.h"
#include "SmithGameOver/GameOverLevel.h"
#include "GameOver_Observer.generated.h"

DECLARE_DELEGATE(FGameOverEvent);


UCLASS()
class SMITH_API AGameOver_Observer : public AActor,public IICallable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameOver_Observer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void OnGameOver() override final;



public:
	FGameOverEvent GameOverEvent;

	AGameOver_TestPawn* callOvject; 

	UGameOverLevel* gameOverLevel;
	

};
