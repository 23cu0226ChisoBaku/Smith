// Fill out your copyright notice in the Description page of Project Settings.
/*

BattleResult.h

Author : MAI ZHICONG(バクチソウ)

Description : バトルリザルト

Update History: 2025/01/29 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_BATTLE_RESULT
#define SMITH_BATTLE_RESULT

#include "Direction.h"

///
/// @brief バトル結果
///
struct FBattleResult
{
  int32 Damage;
  EDirection DamageFrom;

  constexpr FBattleResult()
    : Damage(0)
    , DamageFrom(EDirection::Invalid)
  {}

  FBattleResult(int32 Damage_, EDirection From)
    : Damage(Damage_)
    , DamageFrom(From)
  {}
};

#endif
