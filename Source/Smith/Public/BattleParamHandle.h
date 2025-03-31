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

///
/// @brief 攻撃パラメーターハンドル
///
struct FBattleAttackParamHandle
{
  int32 AttackPoint = 0;
  int32 DefensePoint = 0;
  int32 Level = 0;
  double MotionValue = 0.0;
};

///
/// @brief 防御パラメーターハンドル
///
struct FBattleDefenseParamHandle
{
  int32 DefensePoint = 0;
  int32 Level = 0;
};

#endif

