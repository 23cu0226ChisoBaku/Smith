// Fill out your copyright notice in the Description page of Project Settings.

#include "IBattleLogModelGateway.h"

bool operator==(const FBattleLogModelDTO& Lhs, const FBattleLogModelDTO& Rhs)
{
  return Lhs.LogModelClass == Rhs.LogModelClass;
}
