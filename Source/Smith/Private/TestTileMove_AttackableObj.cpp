// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTileMove_AttackableObj.h"
#include "AttackHandle.h"

// Sets default values
ATestTileMove_AttackableObj::ATestTileMove_AttackableObj()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATestTileMove_AttackableObj::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestTileMove_AttackableObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestTileMove_AttackableObj::OnAttack(const AttackHandle& attack)
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, attack.AttackName);
	}
}

