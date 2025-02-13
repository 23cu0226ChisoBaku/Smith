// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithDungeonDamageCalculator.h"
#include "BattleParamHandle.h"
#include "BattleResult.h"
#include "Math/UnrealMathUtility.h"
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

  // ダメージ軽減率は　攻撃と防御の差で 防御*0.33333(70%)~防御*2(100%)の範囲内でイーズする
  double damageReductionRate = 1.0;
  // 攻撃と防御の差
  int32 atkDefDifference = attacker.AttackPoint - defender.DefensePoint;

  // 差が防御*0.333333(70%)以下の
  if (attacker.AttackPoint < 0 || atkDefDifference < defender.DefensePoint / 3)
  {
    damageReductionRate = 0.7;
  }
  else if (defender.DefensePoint <= 0 || atkDefDifference >= FMath::FloorToInt(StaticCast<float>(defender.DefensePoint) * 1.5f))
  {
    damageReductionRate = 1.0;
  }
  else
  {
    const float funcVariableX = (PI * 0.333333f / StaticCast<float>(defender.DefensePoint) * StaticCast<float>(atkDefDifference))  - (PI * 0.1666666f);
    
    damageReductionRate = FMath::Sin(funcVariableX) * 0.3f + 0.7f;
  }
  
  // 攻撃力　＊　定数^(攻撃者レベル　ー　攻撃を喰らう者のレベル) * 軽減率・小数点切り捨て
  int32 damage = FMath::FloorToInt32(StaticCast<double>(attacker.AttackPoint) * FMath::Pow(m_constant, StaticCast<double>(levelDifference)) * damageReductionRate); 

  FBattleResult result;
  result.Damage = damage;
  return result;
} 
