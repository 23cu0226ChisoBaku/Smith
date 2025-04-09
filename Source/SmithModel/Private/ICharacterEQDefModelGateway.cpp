// Fill out your copyright notice in the Description page of Project Settings.


#include "ICharacterEQDefModelGateway.h"

// Add default functionality here for any ICharacterEQDefModelGateway functions that are not pure virtual.

bool operator==(const FCharaEquipmentDefDTO& Lhs, const FCharaEquipmentDefDTO& Rhs)
{
  return Lhs.UserClass == Rhs.UserClass;
}