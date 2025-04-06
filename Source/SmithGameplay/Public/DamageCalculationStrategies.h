// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageCalculationStrategies.generated.h"

struct FBattleAttackParamHandle;
struct FBattleDefenseParamHandle;
struct FBattleResult;

UENUM()
enum class DamageCalculationStrategyType : uint8
{
	Default UMETA(DisplayName = "Only use Attack power", ToolTip = "攻撃力だけダメージに反映する"),
	ReductionRate UMETA(DisplayName = "Use reduction rate", ToolTip = "constant as base, level diff as exponent (base^(level diff))定数を使って")
};

// DCS(Damage Calculation Strategy)
USTRUCT()
struct FSmithDefaultDCS
{
	GENERATED_BODY()

	FBattleResult operator()(FBattleAttackParamHandle ATKHandle, FBattleDefenseParamHandle DEFHandle) const;
};

USTRUCT()
struct FSmithReductionRateDCS
{
	GENERATED_BODY()

	FBattleResult operator()(FBattleAttackParamHandle ATKHandle, FBattleDefenseParamHandle DEFHandle) const;

	double LevelDiffPowBase = 1.0;
};
