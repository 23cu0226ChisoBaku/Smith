// Fill out your copyright notice in the Description page of Project Settings.


#include "IAttackCauser.h"

// Add default functionality here for any IAttackCauser functions that are not pure virtual.

void IAttackCauser::OnAttackExecuted()
{
  UE_LOG(LogTemp, Warning, TEXT("On Attack Executed by [%s]"), *GetNameSafe(_getUObject()));
}