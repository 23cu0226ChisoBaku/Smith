// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithDungeonDamageCalculator.h"
#include "BattleParamHandle.h"
#include "BattleResult.h"
#include "MLibrary.h"

USmithDungeonDamageCalculator::USmithDungeonDamageCalculator(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_constant(1.0)
{}

void USmithDungeonDamageCalculator::SetConstantNumber(double constant)
{
  m_constant = constant;
}
FBattleResult USmithDungeonDamageCalculator::CalculateDamage( const FBattleAttackParamHandle& attacker, 
                                                              const FBattleDefenseParamHandle& defender) const
{
  int32 levelDifference = attacker.Level - defender.Level;
  // 攻撃力　＊　定数^(攻撃者レベル　ー　攻撃を喰らう者のレベル) 小数点切り捨て
  int32 damage = FMath::FloorToInt32(StaticCast<double>(attacker.AttackPoint) * FMath::Pow(m_constant, StaticCast<double>(levelDifference))); 

  FBattleResult result;
  result.Damage = damage;
  return result;
} 
