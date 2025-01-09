// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGround.h"

// Sets default values
ATestGround::ATestGround()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestGround::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

