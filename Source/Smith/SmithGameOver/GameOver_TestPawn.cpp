// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithGameOver/GameOver_TestPawn.h"

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
	
}

// Called every frame
void AGameOver_TestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameOver_TestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
