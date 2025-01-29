// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ISmithDamageCalculator.h"
#include "SmithDungeonDamageCalculator.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithDungeonDamageCalculator : public UObject, public ISmithDamageCalculator
{
	GENERATED_BODY()

public:
	USmithDungeonDamageCalculator(const FObjectInitializer&);

public:
	void SetConstantNumber(double constant);
	FBattleResult CalculateDamage(const FBattleAttackParamHandle&, const FBattleDefenseParamHandle&) const override;
	
private:
	double m_constant;
};
