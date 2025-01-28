// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameOver.generated.h"

//ÉfÉäÉQÅ[Ég
DECLARE_DELEGATE(FGameOverEventDispatcher);

UCLASS()
class SMITH_API AGameOver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameOver();

	//void InitializeEvent() override final;
	//bool TriggerEvent() override final;
	void TriggerdEvent();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//UPROPERTY(BlueprintAssignable, Category = "Events")
	FGameOverEventDispatcher GameOverEventDispatcher;
};
