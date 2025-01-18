// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithActors/Weapon/Params.h"
#include "SmithActors/Weapon/Skill.h"
#include "Smith/Public/ParamAbsorbable.h"
#include "SmithActors/Weapon/SKillAbsorbable.h"
#include "Debug.h"
#include "Weapon.generated.h"

UCLASS()
class SMITH_API AWeapon : public AActor, public IParamAbsorbable, public ISkillAbsorbable
{
	GENERATED_BODY()

public:
	AWeapon();
	void BeginPlay();
	void Tick(float DeltaTime);

	virtual FParams GetParam() override;
	virtual void AddParam(FParams) override;

	virtual FSkill GetSkills() override;
	virtual void SetSkills(FSkill*) override;

	void SwapSkill(int32, FSkill);
	void Upgrade(IParamAbsorbable*);

private:
	TArray <FSkill> m_skillslots;
	const FParams m_params;
	UPROPERTY(EditAnywhere,Category = "Params|Edit")
	FParams m_currentParams;
	UPROPERTY(EditAnywhere, Category = "Params|Edit")
	FString m_name;
};