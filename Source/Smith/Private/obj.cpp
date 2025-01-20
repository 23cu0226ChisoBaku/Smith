// Fill out your copyright notice in the Description page of Project Settings.


#include "obj.h"
#include "MapObject.h"

// Sets default values
Aobj::Aobj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aobj::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aobj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

