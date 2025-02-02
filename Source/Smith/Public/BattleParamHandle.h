// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_BATTLE_PARAM_HANDLE
#define SMITH_BATTLE_PARAM_HANDLE

#include "CoreMinimal.h"

struct FBattleAttackParamHandle
{
  int32 AttackPoint;
  int32 CriticalPoint;
  int32 Level;
  double MotionValue;

  FBattleAttackParamHandle()
    : AttackPoint(0)
    , CriticalPoint(0)
    , Level(0)
    , MotionValue(0)
  {}
};

struct FBattleDefenseParamHandle
{
  int32 DefensePoint;
  int32 Level;

  FBattleDefenseParamHandle()
    : DefensePoint(0)
    , Level(0)
  {}
};

#endif

