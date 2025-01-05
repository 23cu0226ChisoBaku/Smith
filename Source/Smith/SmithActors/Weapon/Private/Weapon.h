// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SMITH_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	void BeginPlay();
	void Tick(float DeltaTime);



private:


};