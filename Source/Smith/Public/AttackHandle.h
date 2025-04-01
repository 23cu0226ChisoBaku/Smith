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

#ifndef SMITH_ATTACK_HANDLE
#define SMITH_ATTACK_HANDLE

#include "Direction.h"

struct SMITH_API FAttackHandle
{
	static const FAttackHandle NullHandle;

	int32 AttackPower;
	int32 Level;
	double MotionValue;
	EDirection AttackFrom;

	constexpr FAttackHandle()
		: AttackPower(0)
		, Level(0)
		, MotionValue(1.0)
		, AttackFrom(EDirection::Invalid)
	{}
};

#endif

