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

FParams USmithUpgradeMaterial::GetParam_Interface() const
{
  return Param;
}

void USmithUpgradeMaterial::AddParam(FParams)
{
  unimplemented();
}

UTexture2D* USmithUpgradeMaterial::GetIconImage() const
{
  return Icon;
}

FColor USmithUpgradeMaterial::GetLabelColor() const
{
  int32 maxParam = -999;
  FColor resultColor = FColor::Black;

  if (maxParam < Param.HP)
  {
    maxParam = Param.HP;
    resultColor = FColor::Green;  
  }

  if (maxParam < Param.ATK)
  {
    maxParam = Param.ATK;
    resultColor = FColor::Red;
  }

  if (maxParam < Param.DEF)
  {
    maxParam = Param.DEF;
    resultColor = FColor::Blue;
  }

  if (maxParam < Param.CRT)
  {
    maxParam = Param.CRT;
    resultColor = FColor::Yellow;
  }

  resultColor.A = StaticCast<uint8>(255.0 * 0.9);
  return resultColor;
}

FString USmithUpgradeMaterial::GetName() const
{
  return GetName_Log();
}

FString USmithUpgradeMaterial::GetDescription() const
{
  return Description;
}

EBattleLogType USmithUpgradeMaterial::GetType_Log() const
{
  return EBattleLogType::Item;
}