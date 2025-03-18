// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithDamageSubsystem.h"

#include "IAttackable.h"
#include "IAttackCauser.h"
#include "AttackHandle.h"

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

void USmithDamageSubsystem::ApplyDamage(AActor* DamageInstigator, AActor* DamageCauser, AttackHandle&& Handle) const
{
  IAttackable* attackTarget = Cast<IAttackable>(DamageCauser);
  if (attackTarget != nullptr)
  {
    attackTarget->OnAttack(::MoveTemp(Handle));
  }

  IAttackCauser* attackCauser = Cast<IAttackCauser>(DamageInstigator);
  if (attackCauser != nullptr)
  {
    attackCauser->OnAttackExecuted();
  }
  
}