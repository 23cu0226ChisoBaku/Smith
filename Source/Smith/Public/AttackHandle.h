// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_ATKHDL
#define SMITH_ATKHDL

#include "CoreMinimal.h"

class ISmithBattleLogger;

struct SMITH_API FAttackHandle
{
	static const FAttackHandle NullHandle;

	ISmithBattleLogger* Attacker;
	int32 AttackPower;
	int32 CriticalPower;
	int32 Level;
	double MotionValue;

	constexpr FAttackHandle()
		: Attacker(nullptr)
		, AttackPower(0)
		, CriticalPower(0)
		, Level(0)
		, MotionValue(0)
	{}
};

struct SMITH_API AttackHandle
{
	static const AttackHandle NullHandle;

	ISmithBattleLogger* Attacker;
	int32 AttackPower;
};

const AttackHandle AttackHandle::NullHandle{};

#endif

