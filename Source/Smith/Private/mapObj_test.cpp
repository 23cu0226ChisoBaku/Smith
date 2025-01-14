// Fill out your copyright notice in the Description page of Project Settings.


#include "mapObj_test.h"
#include "Debug.h"

// Sets default values
AmapObj_test::AmapObj_test()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AmapObj_test::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AmapObj_test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MDebug::LogWarning("test");

}

// Called to bind functionality to input
void AmapObj_test::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

