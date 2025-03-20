// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMinimap.h"
#include "SmithMinimapTileEntry.h"
#include "SmithMinimapTileView.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "IMinimapDisplayable.h"
#include "MapCoord.h"
#include "MLibrary.h"


void USmithMinimap::NativeConstruct()
{
  Super::NativeConstruct();
}
void USmithMinimap::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);
}
void USmithMinimap::AssignMapData(const TArray<UObject*>& mapItems, uint8 row, uint8 column)
{
  if (Minimap == nullptr)
  {
    return;
  }

  const int32 tileCount = StaticCast<int32>(row) * StaticCast<int32>(column);

  Minimap->InitMinimap(mapItems, row, column);
  adjustMinimapPosition();
  SetVisibility(true);
}
void USmithMinimap::ReplaceTiles(const TArray<UObject*>& tileItems, const FMapCoord& originCoord, uint8 sizeX, uint8 sizeY)
{
  if (Minimap == nullptr)
  {
    return;
  }

  const int32 replaceTileCount = StaticCast<int32>(sizeX) * StaticCast<int32>(sizeY);
  if (tileItems.Num() != replaceTileCount)
  {
    return;
  }

  for (uint8 row = 0u; row < sizeY; ++row)
  {
    for (uint8 column = 0u; column < sizeX; ++column)
    {
      const uint8 setTileX = originCoord.x + column;
      const uint8 setTileY = originCoord.y + row;
      // TODO
      const int32 tileIdx = StaticCast<int32>(row) * StaticCast<int32>(sizeX) + StaticCast<int32>(column);
      UObject* tileItem = tileItems[tileIdx];

      Minimap->ReplaceTile(tileItem, setTileX, setTileY);
    }
  }

}
void USmithMinimap::UpdateMinimap(UObject* displayable, const FMapCoord& originCoord, uint8 sizeX, uint8 sizeY)
{
  if (Minimap == nullptr)
  {
    return;
  }

  if (displayable == nullptr)
  {
    return;
  }

  for (uint8 row = 0u; row < sizeY; ++row)
  {
    for (uint8 column = 0u; column < sizeX; ++column)
    {
      const uint8 setTileX = originCoord.x + column;
      const uint8 setTileY = originCoord.y + row;

      Minimap->SetTile(displayable, setTileX, setTileY);
    }
  }
}
void USmithMinimap::SetVisibility(bool bIsVisible)
{
  const ESlateVisibility visibility = bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
  if (Minimap != nullptr)
  {
    Minimap->SetVisibility(visibility);
  }

  if (MinimapBackground != nullptr)
  {
    MinimapBackground->SetVisibility(visibility);
  }
}
void USmithMinimap::ResetMap()
{
  if (Minimap != nullptr)
  {
    Minimap->EmptyTiles();
  }

  SetVisibility(false);
}

void USmithMinimap::adjustMinimapPosition()
{
  if (MinimapBackground != nullptr)
  {
    UCanvasPanelSlot* backgroundCanvasSlot = Cast<UCanvasPanelSlot>(MinimapBackground->Slot);
    UCanvasPanelSlot* minimapCanvasSlot = Cast<UCanvasPanelSlot>(Minimap->Slot);
    if (backgroundCanvasSlot != nullptr && minimapCanvasSlot != nullptr)
    {
      FVector2D backgroundSize = backgroundCanvasSlot->GetSize();
      FVector2D minimapSize = minimapCanvasSlot->GetSize();

      const double minimapNewLeft = (backgroundSize.X - minimapSize.X) * 0.5;
      const double minimapNewTop = (backgroundSize.Y - minimapSize.Y) * 0.5;
      const FVector2D minimapNewPosition = backgroundCanvasSlot->GetPosition() + FVector2D(minimapNewLeft, minimapNewTop);

      minimapCanvasSlot->SetPosition(minimapNewPosition);
    }
  }
}