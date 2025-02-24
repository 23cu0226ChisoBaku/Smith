// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapObjectDecorator.h"
#include "SmithMapObjDisplayRow.h"

void USmithMapObjectDecorator::UpdateTile(uint8 tileData, uint8 x, uint8 y, UTexture2D*)
{
  if (!m_mapObjDisplayTable.Contains(tileData))
  {
    return;
  }

  if (m_minimapDecorator != nullptr)
  {
    m_minimapDecorator->UpdateTile(tileData, x, y, m_mapObjDisplayTable[tileData]);
  }
}

void USmithMapObjectDecorator::BeginPlay()
{
  if (MapObjectDisplayRow == nullptr)
  {
    return;
  }

  TArray<FSmithMapObjDisplayRow*> tableRows;
  MapObjectDisplayRow->GetAllRows<FSmithMapObjDisplayRow>(nullptr, tableRows);

  for (int32 i = 0; i < tableRows.Num(); ++i)
  {
    const uint8 convertedKey = ConvertToTileType(tableRows[i]->ObjectType);
    if (convertedKey == Invalid || m_mapObjDisplayTable.Contains(convertedKey))
    {
      continue;
    }

    m_mapObjDisplayTable.Emplace(convertedKey, tableRows[i]->ObjDisplayTexture);
  }
}