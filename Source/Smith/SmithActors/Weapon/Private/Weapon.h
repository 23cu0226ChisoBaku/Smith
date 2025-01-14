// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithActors/Weapon/Private/Weapon_Base.h"
#include "SmithActors/Weapon/Params.h"
#include "Smith/Public/ParamAbsorbable.h"
#include "Debug.h"
#include "Weapon.generated.h"

UCLASS()
class SMITH_API AWeapon : public AActor, public IParamAbsorbable
{
	GENERATED_BODY()

public:
	AWeapon();
	void BeginPlay();
	void Tick(float DeltaTime);

	virtual FParams GetParam() override;

	void SetParam(FParams);

	void SwapSkill(int, Skill);
	void Upgrade(IParamAbsorbable*);

private:
	//UPROPERTY(EditAnywhere)
	TArray <Skill> m_skillslots;
	UPROPERTY(EditAnywhere)
	FParams m_params;
};