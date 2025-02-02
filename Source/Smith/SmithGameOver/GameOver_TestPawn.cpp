// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithGameOver/GameOver_TestPawn.h"
#include "Debug.h"

// Sets default values
AGameOver_TestPawn::AGameOver_TestPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameOver_TestPawn::BeginPlay()
{
	Super::BeginPlay();

	GameOverActor = NewObject<AGameOver_Observer>(this);
	if (GameOverActor == nullptr)
	{
		MDebug::LogError("GameOverActor error");
	}

}

// Called every frame
void AGameOver_TestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gameover == true)
	{
		OnDead();
	}
}

// Called to bind functionality to input
void AGameOver_TestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameOver_TestPawn::OnDead()
{
	if(GameOverActor!= nullptr)
	{
		GameOverActor->OnGameOver();
	}
}

