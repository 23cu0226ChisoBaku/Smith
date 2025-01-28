// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithGameOver/GameOver.h"

// Sets default values
AGameOver::AGameOver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameOver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameOver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameOver::TriggerdEvent()
{
	//GameOverEventDispatcher.Broadcast();
}


