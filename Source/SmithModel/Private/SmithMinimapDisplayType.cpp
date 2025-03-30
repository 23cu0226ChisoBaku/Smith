// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMinimapDisplayType.h"

USmithMinimapDisplayType::USmithMinimapDisplayType()
  : m_displayTexture(nullptr)
{ }

void USmithMinimapDisplayType::BeginDestroy()
{
  Super::BeginDestroy();
}

void USmithMinimapDisplayType::SetTexture(UTexture2D* texture)
{
  m_displayTexture = texture;
}

UTexture2D* USmithMinimapDisplayType::GetMinimapDisplayTexture_Implementation()
{
  return m_displayTexture;
}