// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMinimapTileView.h"
#include "Components/TileView.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "SmithMinimapTileEntry.h"
#include "IMinimapDisplayable.h"
#include "MapCoord.h"
#include "MLibrary.h"

namespace TileView::Private
{
  constexpr float TILE_WIDTH = 8.0f;
  constexpr float TILE_HEIGHT = 8.0f;
}

USmithMinimapTileView::USmithMinimapTileView(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , MinimapTiles(nullptr)
  , m_minimapRow(0u)
  , m_minimapColumn(0u)
{ }

void USmithMinimapTileView::NativeConstruct()
{
  Super::NativeConstruct();

  using namespace TileView::Private;
  if (MinimapTiles != nullptr)
  {
    MinimapTiles->SetEntryWidth(TILE_WIDTH);
    MinimapTiles->SetEntryHeight(TILE_HEIGHT);
  }
}

void USmithMinimapTileView::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);
}

void USmithMinimapTileView::InitMinimap(const TArray<UObject*>& mapTiles, uint8 row, uint8 column)
{
  if (MinimapTiles == nullptr)
  {
    return;
  }

  const int32 tileCapacity = StaticCast<int32>(row) * StaticCast<int32>(column); 
  check(mapTiles.Num() == tileCapacity);

  MinimapTiles->SetListItems(mapTiles);
  m_minimapRow = row;
  m_minimapColumn = column;

  using namespace TileView::Private;
  UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(Slot);
  if (canvasSlot != nullptr)
  {
    FVector2D newSize(StaticCast<double>(m_minimapColumn) * TILE_WIDTH, StaticCast<double>(m_minimapRow) * TILE_HEIGHT);
    canvasSlot->SetSize(newSize);
  }

  canvasSlot = Cast<UCanvasPanelSlot>(MinimapTiles->Slot);
  if (canvasSlot != nullptr)
  {
    FVector2D newSize(StaticCast<double>(m_minimapColumn) * TILE_WIDTH, StaticCast<double>(m_minimapRow) * TILE_HEIGHT);
    canvasSlot->SetSize(newSize);
  }
}

void USmithMinimapTileView::ReplaceTile(UObject* tileItem, uint8 x, uint8 y)
{
  replaceTileItemImpl(tileItem, x, y);
}

void USmithMinimapTileView::SetTile(UObject* tileItem, uint8 x, uint8 y)
{
  setTileImpl(tileItem, x, y);
}

void USmithMinimapTileView::SetTileVisible(uint8 x, uint8 y)
{
  USmithMinimapTileEntry* tileEntry = getTileEntryInternal(x, y);
  if (tileEntry == nullptr)
  {
    return;
  }

  tileEntry->SetVisibility(ESlateVisibility::Visible);
}

void USmithMinimapTileView::EmptyTiles()
{
  if (MinimapTiles == nullptr)
  {
    return;
  }

  MinimapTiles->ClearListItems();
  // TODO Entry Widget Index mismatch with Item Index
  // Call this to reset Index
  MinimapTiles->RegenerateAllEntries();
}

USmithMinimapTileEntry* USmithMinimapTileView::getTileEntryInternal(uint8 x, uint8 y) const
{
  if (MinimapTiles == nullptr)
  {
    return nullptr;
  }

  if ((x >= m_minimapColumn) || (y >= m_minimapRow))
  {
    return nullptr;
  }

  const TArray<UUserWidget*> displayedItems = MinimapTiles->GetDisplayedEntryWidgets();
  const int32 idx = StaticCast<int32>(y) * StaticCast<int32>(m_minimapColumn) + StaticCast<int32>(x);

  // これは不可能だと思うが、念のため
  if (idx >= displayedItems.Num())
  {
    return nullptr;
  }

  return Cast<USmithMinimapTileEntry>(displayedItems[idx]);
}

void USmithMinimapTileView::setTileImpl(UObject* tileItem, uint8 x, uint8 y)
{
  if (!::IsValid(tileItem))
  {
    return;
  }

  USmithMinimapTileEntry* entryWidget = getTileEntryInternal(x, y);
  if (entryWidget == nullptr)
  {
    return;
  }

  // TODO BP class もこのインタフェースを継承したため、Castだと失敗する
  if (tileItem->Implements<UMinimapDisplayable>())
  {
    entryWidget->SetTileTexture(IMinimapDisplayable::Execute_GetMinimapDisplayTexture(tileItem));
  }

}

void USmithMinimapTileView::replaceTileItemImpl(UObject* tileItem, uint8 x, uint8 y)
{
  if (MinimapTiles == nullptr)
  {
    return;
  }

  if (!::IsValid(tileItem))
  {
    return;
  }

  const TArray<UObject*> entryItems = MinimapTiles->GetListItems();
  const int32 idx = StaticCast<int32>(y) * StaticCast<int32>(m_minimapColumn) + StaticCast<int32>(x);

  if (idx >= entryItems.Num())
  {
    return;
  }

  TArray<UObject*> newItemList = entryItems;
  newItemList.RemoveAt(idx);
  newItemList.Insert(tileItem, idx);

  MinimapTiles->SetListItems(newItemList);
}
