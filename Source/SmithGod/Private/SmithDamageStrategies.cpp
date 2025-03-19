// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithDamageStrategies.h"

#include "GameFramework/Actor.h"
#include "SmithDamageSubsystem.h"
#include "SmithBattleLogWorldSubsystem.h"

void SmithDefaultDamageStrategy::operator()()
{
  if (DamageSubsystem != nullptr)
  {
    DamageSubsystem->ApplyDamage(Instigator, Causer, ::MoveTemp(Handle), FromDirection);
  }
}
