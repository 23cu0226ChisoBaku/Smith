// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapTileDecorator.h"
#include "SmithMapTileDisplayRow.h"

void USmithMapTileDecorator::UpdateTile(uint8 tileData, uint8 x, uint8 y, UTexture2D*)
{
  if (!m_mapTileDisplayTable.Contains(tileData))
  {
    return;
  }

  if (m_minimapDecorator != nullptr)
  {
    m_minimapDecorator->UpdateTile(tileData, x, y, m_mapTileDisplayTable[tileData]);
  }
}

void USmithMapTileDecorator::BeginPlay()
{
  if (MapTileDisplayRow == nullptr)
  {
    return;
  }

  TArray<FSmithMapTileDisplayRow*> tableRows;
  MapTileDisplayRow->GetAllRows<FSmithMapTileDisplayRow>(nullptr, tableRows);

  for (int32 i = 0; i < tableRows.Num(); ++i)
  {
    const uint8 convertedKey = ConvertToTileType(tableRows[i]->ObjectType);
    if (convertedKey == Invalid || m_mapTileDisplayTable.Contains(convertedKey))
    {
      continue;
    }

    m_mapTileDisplayTable.Emplace(convertedKey, tableRows[i]->TileDisplayTexture);
  }
}
