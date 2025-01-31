// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithGameOver/GameOver_Observer.h"
#include "Debug.h"

// Sets default values
AGameOver_Observer::AGameOver_Observer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void AGameOver_Observer::BeginPlay()
{
	Super::BeginPlay();

	gameOverLevel = NewObject<AGameOverLevel>(this);
	if (gameOverLevel != nullptr)
	{
		GameOverEvent.BindUObject(gameOverLevel, &AGameOverLevel::LoadLevel);
	}
	else
	{
		MDebug::LogError("GameOverLevel error");
	}
}

// Called every frame
void AGameOver_Observer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameOver_Observer::OnGameOver()
{
	if(GameOverEvent.IsBound())
	{
		GameOverEvent.Execute();
	}
}

