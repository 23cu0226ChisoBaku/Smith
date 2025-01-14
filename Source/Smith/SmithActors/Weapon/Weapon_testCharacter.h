// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Weapon_testCharacter.generated.h"

class AWeapon;

UCLASS()
class SMITH_API AWeapon_testCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWeapon_testCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ‚Æ‚è‚ ‚¦‚¸
	void GetActorOfWeapon();
	void SetWeapon(AWeapon* pWeapon);
	void UpdateWeaponParams();
	void GetAxis(float);

	AWeapon* p_weapon;
};
