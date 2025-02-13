// Fill out your copyright notice in the Description page of Project Settings.
/*

BattleParamHandle.h

Author : MAI ZHICONG(バクチソウ)

Description : バトル用ハンドル

Update History: 2025/01/29  作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_BATTLE_PARAM_HANDLE
#define SMITH_BATTLE_PARAM_HANDLE

#include "CoreMinimal.h"

///
/// @brief 攻撃パラメーターハンドル
///
struct FBattleAttackParamHandle
{
  int32 AttackPoint;
  int32 CriticalPoint;
  int32 DefensePoint;
  int32 Level;
  double MotionValue;

  constexpr FBattleAttackParamHandle()
    : AttackPoint(0)
    , CriticalPoint(0)
    , Level(0)
    , MotionValue(0)
  {}
};

///
/// @brief 防御パラメーターハンドル
///
struct FBattleDefenseParamHandle
{
  int32 DefensePoint;
  int32 Level;

  constexpr FBattleDefenseParamHandle()
    : DefensePoint(0)
    , Level(0)
  {}
};

#endif

