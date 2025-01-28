// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISmithBattleLogger.h"


/**
 * 
 */

#ifndef SMITH_ATKHDL
#define SMITH_ATKHDL

struct SMITH_API AttackHandle
{
	static const AttackHandle NullHandle;

	ISmithBattleLogger* Attacker;
	int32 AttackPower;
};

#endif

