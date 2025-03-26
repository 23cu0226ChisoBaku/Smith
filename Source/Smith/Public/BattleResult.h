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


//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class ISmithBattleLogger;

///
/// @brief バトル結果
///
struct FBattleResult
{
  ///
  /// @brief 戦う相手のLog出力用ポインタ
  ///
  ISmithBattleLogger* ResultLogger;
  ///
  /// @brief ダメージ
  ///
  int32 Damage;

  constexpr FBattleResult()
    : ResultLogger(nullptr)
    , Damage(0)
  {}

  FBattleResult(ISmithBattleLogger* ResultLogger_, int32 Damage_)
    : ResultLogger(ResultLogger_)
    , Damage(Damage_)
  {}
};

#endif
