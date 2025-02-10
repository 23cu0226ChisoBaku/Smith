// Fill out your copyright notice in the Description page of Project Settings.
/*

BattleResult.h

Author : MAI ZHICONG(バクチソウ)

Description : [ファイル説明]

Update History: [20xx/xx/xx] [コメント]

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_BATTLE_RESULT
#define SMITH_BATTLE_RESULT

#include "CoreMinimal.h"
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
  ISmithBattleLogger* ResultLogger;
  int32 Damage;

  constexpr FBattleResult()
    : ResultLogger(nullptr)
    , Damage(0)
  {}
};

#endif
