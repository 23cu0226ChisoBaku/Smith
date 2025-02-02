// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithWeapon.h"
#include "ParamAbsorbable.h"

#include "MLibrary.h"

USmithWeapon::USmithWeapon(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_weaponLevel(1)
{ }

void USmithWeapon::BeginDestroy()
{
  Super::BeginDestroy();
}

void USmithWeapon::Upgrade(IParamAbsorbable* absorbItem)
{
  if (!IS_UINTERFACE_VALID(absorbItem))
  {
    MDebug::LogError("Upgrade failed --- absorbItem invalid");
    return;
  }

  WeaponParam += absorbItem->GetParam();
  if (OnUpgrade.IsBound())
  {
    OnUpgrade.Broadcast(absorbItem->GetParam());
  }
  ++m_weaponLevel;
}

void USmithWeapon::SetParam(FParams param)
{
  WeaponParam = param;
}

FParams USmithWeapon::GetParam() const
{
  return WeaponParam;
}

int32 USmithWeapon::GetLevel() const
{
  return m_weaponLevel;
}
