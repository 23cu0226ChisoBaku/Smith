// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageCalculationStrategies.h"

#include "BattleParamHandle.h"
#include "BattleResult.h"
#include "Direction.h"

FBattleResult FSmithDefaultDCS::operator()(FBattleAttackParamHandle ATKHandle, FBattleDefenseParamHandle DEFHandle) const
{
  return FBattleResult{ATKHandle.AttackPoint, EDirection::Invalid};
}

FBattleResult FSmithReductionRateDCS::operator()(FBattleAttackParamHandle ATKHandle, FBattleDefenseParamHandle DEFHandle) const
{
  // ダメージ軽減率は　攻撃と防御の差で 防御*0.33333(70%)~防御*2(100%)の範囲内でイーズする
  double damageScaling = 1.0;

  const int32 levelDiff = ATKHandle.Level - DEFHandle.Level;
  const int32 atkDefDiff = ATKHandle.AttackPoint - DEFHandle.DefensePoint;

  // 差が防御*0.333333(70%ダメージ※30%軽減率)以下
  if (ATKHandle.AttackPoint < 0 || atkDefDiff < DEFHandle.DefensePoint / 3)
  {
    damageScaling = 0.7;
  }
  // 差が防御*2(100%※0%軽減率)以上
  else if (DEFHandle.DefensePoint <= 0 || atkDefDiff >= FMath::FloorToInt(StaticCast<float>(DEFHandle.DefensePoint)))
  {
    damageScaling = 1.0;
  }
  else
  {
    const float funcVariableX = (PI * 0.3f / StaticCast<float>(DEFHandle.DefensePoint) * StaticCast<float>(atkDefDiff))  - (PI * 0.1f);
    
    damageScaling = FMath::Sin(funcVariableX) * 0.3f + 0.7f;
  }
  
  // 攻撃力　＊　定数^(攻撃者レベル　ー　攻撃を喰らう者のレベル) * 軽減率 * モーション値  ※小数点切り捨て
  const int32 damage = FMath::FloorToInt32(StaticCast<double>(ATKHandle.AttackPoint) * FMath::Pow(LevelDiffPowBase, StaticCast<double>(levelDiff)) * damageScaling * ATKHandle.MotionValue); 

  return FBattleResult{damage, EDirection::Invalid};
}

