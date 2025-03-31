// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeMaterial.h"

#include <limits>
#include "MLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithUpgradeMaterial)

USmithUpgradeMaterial::USmithUpgradeMaterial(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_labelColor(FColor::White)
{ }

void USmithUpgradeMaterial::PostInitProperties()
{
  Super::PostInitProperties();
  int32 maxParam = std::numeric_limits<int32>::min();
  if (maxParam < Param.HP)
  {
    maxParam = Param.HP;
    m_labelColor = FColor::Green;  
  }

  if (maxParam < Param.ATK)
  {
    maxParam = Param.ATK;
    m_labelColor = FColor::Red;
  }

  if (maxParam < Param.DEF)
  {
    maxParam = Param.DEF;
    m_labelColor = FColor::Blue;
  }

  if (maxParam < Param.CRT)
  {
    maxParam = Param.CRT;
    m_labelColor = FColor::Yellow;
  }

  m_labelColor.A = StaticCast<uint8>(255.0 * 0.9);
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
  return m_labelColor;
}

FString USmithUpgradeMaterial::GetName() const
{
  return Name;
}

FString USmithUpgradeMaterial::GetDescription() const
{
  return Description;
}
