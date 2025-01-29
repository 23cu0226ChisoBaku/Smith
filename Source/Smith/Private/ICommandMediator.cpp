// Fill out your copyright notice in the Description page of Project Settings.

#include "ICommandMediator.h"

bool SendAttackCommand(AActor*, ICanMakeAttack*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&, bool bAttackEvenNoTarget)
{
  return false;
}