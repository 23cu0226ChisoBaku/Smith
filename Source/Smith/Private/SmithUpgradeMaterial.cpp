// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeMaterial.h"
#include "ICanPick.h"
#include "MLibrary.h"

USmithUpgradeMaterial::USmithUpgradeMaterial(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ 
  // TODO Test Code
  Param = FParams{0,3,0,0};
}

void USmithUpgradeMaterial::BeginDestroy()
{
  Super::BeginDestroy();
}

FParams USmithUpgradeMaterial::GetParam()
{
  return Param;
}

void USmithUpgradeMaterial::AddParam(FParams)
{
  unimplemented();
}

void USmithUpgradeMaterial::onPickImpl(ICanPick* picker)
{
  if (!IS_UINTERFACE_VALID(picker))
  {
    return;
  }

  picker->PickUpMaterial(this);
}