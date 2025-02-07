// Fill out your copyright notice in the Description page of Project Settings.

#include "IAttackable.h"
#include "BattleParamHandle.h"

FBattleDefenseParamHandle IAttackable::GetDefenseParam() const
{
  FBattleDefenseParamHandle param;
  param.Level = 1;
  return param;
}

