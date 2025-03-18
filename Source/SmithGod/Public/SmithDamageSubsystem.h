// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "SmithDamageSubsystem.generated.h"

struct AttackHandle;

UCLASS()
class SMITHGOD_API USmithDamageSubsystem final: public UWorldSubsystem
{
	GENERATED_BODY()

public:
	//---Begin of USubsystem Interface
	bool ShouldCreateSubsystem(UObject* Outer) const override final;
	//---End of USubsystem Interface

	void ApplyDamage(AActor* DamageInstigator, AActor* DamageCauser, AttackHandle&& Handle) const;
	
};
