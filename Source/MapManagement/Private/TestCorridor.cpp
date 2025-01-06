// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCorridor.h"

// Sets default values
ATestCorridor::ATestCorridor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestCorridor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCorridor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

