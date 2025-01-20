// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithActors/TurnMngObj/Public/SmithEnemy.h"
#include "ISkillable.h"
#include "HealGolem.generated.h"

class IAttackable;
/**
 *
 */
UCLASS() 
class SMITH_API AHealGolem : public ASmithEnemy,	public ISkillable
{
	GENERATED_BODY()

public:
	AHealGolem();

public:
	void Tick(float DeltaTime) override final;
	void OnSkill();

private:
	AActor *target;
	int32 m_healPoint;
};
