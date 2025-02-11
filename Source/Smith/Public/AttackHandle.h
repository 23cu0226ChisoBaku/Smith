// Fill out your copyright notice in the Description page of Project Settings.
/*

AttackHandle.h

Author : MAI ZHICONG(バクチソウ)

Description : 攻撃ハンドル

Update History: 2024/12/08	作成
								2025/01/29  FAttackHandle追加作成（ログを出すため）

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_ATK_HDL
#define SMITH_ATK_HDL

#include "CoreMinimal.h"
#include "Direction.h"
//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class ISmithBattleLogger;

struct SMITH_API FAttackHandle
{
	static const FAttackHandle NullHandle;

	ISmithBattleLogger* Attacker;
	int32 AttackPower;
	int32 CriticalPower;
	int32 Level;
	double MotionValue;
	EDirection AttackFrom;

	constexpr FAttackHandle()
		: Attacker(nullptr)
		, AttackPower(0)
		, CriticalPower(0)
		, Level(0)
		, MotionValue(0)
		, AttackFrom(EDirection::Invalid)
	{}
};

struct SMITH_API AttackHandle
{
	static const AttackHandle NullHandle;

	ISmithBattleLogger* Attacker;
	int32 AttackPower;
	EDirection AttackFrom;

};


#endif

