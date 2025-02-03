// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_BATTLE_RESULT
#define SMITH_BATTLE_RESULT

#include "CoreMinimal.h"

class ISmithBattleLogger;

struct FBattleResult
{
  ISmithBattleLogger* ResultLogger;
  int32 Damage;

  FBattleResult()
    : ResultLogger(nullptr)
    , Damage(0)
  {}
};

#endif
