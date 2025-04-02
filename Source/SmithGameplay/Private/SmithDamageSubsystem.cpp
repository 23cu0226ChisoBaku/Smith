// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithDamageSubsystem.h"

#include "IAttackable.h"
#include "IAttackCauser.h"
#include "AttackHandle.h"
#include "Direction.h"
#include "DamageCalculationStrategies.h"

#include "SmithBattleLogWorldSubsystem.h"

#include "SmithTurnBattleWorldSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithDamageSubsystem)

bool USmithDamageSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
  if (!Super::ShouldCreateSubsystem(Outer))
  {
    return false;
  }

  UWorld* worldOuter = Cast<UWorld>(Outer);
  if (worldOuter != nullptr)
  {
    // バトルレベルだけ作る
    ASmithTurnBattleWorldSettings* smithWorldSettings = Cast<ASmithTurnBattleWorldSettings>(worldOuter->GetWorldSettings());
    if (smithWorldSettings != nullptr)
    {
      return smithWorldSettings->IsBattleLevel();
    }
  }

  return false;
}

void USmithDamageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  // Logシステムを先に初期化させる
  Collection.InitializeDependency(USmithBattleLogWorldSubsystem::StaticClass());
  m_logSystem = GetWorld()->GetSubsystem<USmithBattleLogWorldSubsystem>();

  m_pimpl = ::MakeUnique<DamageCalculationModel<FSmithDefaultDCS>>(FSmithDefaultDCS{});
}

void USmithDamageSubsystem::Deinitialize()
{
  m_pimpl.Reset();

  Super::Deinitialize();
}

void USmithDamageSubsystem::ApplyDamage(AActor* DamageInstigator, AActor* DamageCauser, const FAttackHandle& Handle, EDirection FromDirection) const
{
  IAttackable* attackTarget = Cast<IAttackable>(DamageCauser);
  if ((attackTarget != nullptr) && (!attackTarget->IsDefeated()))
  {
    FBattleAttackParamHandle attackParam{};
    attackParam.AttackPoint = Handle.AttackPower;
    attackParam.Level = Handle.Level;
    attackParam.MotionValue = Handle.MotionValue;
    
    FBattleResult result = m_pimpl->CalculateDamage(attackParam, attackTarget->GetDefenseParam());
    result.DamageFrom = FromDirection;
 
    attackTarget->OnAttack(result);

    if (m_logSystem != nullptr)
    {
      m_logSystem->SendAttackLog(DamageInstigator, DamageCauser);
      m_logSystem->SendDamageLog(DamageCauser, result.Damage);
    }
    
    if (attackTarget->IsDefeated())
    {
      attackTarget->OnDefeated();
      m_logSystem->SendDefeatedLog(DamageCauser);
    }
  }

  IAttackCauser* attackCauser = Cast<IAttackCauser>(DamageInstigator);
  if (attackCauser != nullptr)
  {
    attackCauser->OnAttackExecuted();
  }


  
}