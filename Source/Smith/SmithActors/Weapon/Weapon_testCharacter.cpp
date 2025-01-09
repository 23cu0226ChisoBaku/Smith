// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithActors/Weapon/Weapon_testCharacter.h"

// Sets default values
AWeapon_testCharacter::AWeapon_testCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeapon_testCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon_testCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWeapon_testCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

