// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapDisplayTypeFactory.h"

#include "SmithMinimapDisplayType.h"
#include "SmithMinimapDisplayProductRow.h"

IMinimapDisplayable* UMinimapDisplayTypeFactory::GetDisplayable(EMinimapDisplayType type)
{
  UTexture2D* texture = findProductFromDataTable(type);
  if (texture == nullptr)
  {
    return nullptr;
  }

  USmithMinimapDisplayType* displayTypeFlyweightObj = NewObject<USmithMinimapDisplayType>(this, USmithMinimapDisplayType::StaticClass());
  if (displayTypeFlyweightObj == nullptr)
  {
    return nullptr;
  }

  displayTypeFlyweightObj->SetTexture(texture);
  m_typeCaches.Emplace(type, displayTypeFlyweightObj); 
  return displayTypeFlyweightObj;
}

UTexture2D* UMinimapDisplayTypeFactory::findProductFromDataTable(EMinimapDisplayType type)
{
  if (FactoryProducts == nullptr)
  {
    return nullptr;
  }

  TArray<FSmithMinimapDisplayProductRow*> tableRows;
  FactoryProducts->GetAllRows<FSmithMinimapDisplayProductRow>(nullptr, tableRows);

  for (const auto& row : tableRows)
  {
    if (row->Type == type)
    {
      return row->DisplayTexture;
    }
  }

  return nullptr;
}