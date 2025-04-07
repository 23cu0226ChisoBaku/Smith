// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithWeapon.h"

#include "ParamAbsorbable.h"
#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithWeapon)

USmithWeapon::USmithWeapon(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_weaponLevel(1)
{ }

void USmithWeapon::BeginDestroy()
{
  Super::BeginDestroy();

  WeaponParam = FParams{};
}

void USmithWeapon::Enhance(IParamAbsorbable* absorbItem)
{
  if (absorbItem == nullptr)
  {
    MDebug::LogError("Upgrade failed --- absorbItem invalid");
    return;
  }

  FParams absorbParam = absorbItem->GetParam();
  WeaponParam += absorbParam;
  if (OnParamUpdated.IsBound())
  {
    OnParamUpdated.Broadcast(absorbParam);
  }
  
  ++m_weaponLevel;
}

void USmithWeapon::OnEnhanced()
{
  using namespace MLibrary::UE::Audio;
  AudioKit::PlaySE(TEXT("Hit_Iron_1"));
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

