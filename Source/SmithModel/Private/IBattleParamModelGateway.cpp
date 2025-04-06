// Fill out your copyright notice in the Description page of Project Settings.


#include "IBattleParamModelGateway.h"

bool operator==(const FBattleParamDTO& Lhs, const FBattleParamDTO& Rhs)
{
  return Lhs.ParamClass == Rhs.ParamClass;
}
