// Fill out your copyright notice in the Description page of Project Settings.

#include "IAttackable.h"

#include "BattleParamHandle.h"

FBattleDefenseParamHandle IAttackable::GetDefenseParam() const
{
  FBattleDefenseParamHandle param = 
  {
    .DefensePoint = 0,
    .Level = 1
  };
  return param;
}

