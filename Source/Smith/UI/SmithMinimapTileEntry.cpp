// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMinimapTileEntry.h"
#include "Components/Image.h"
#include "IMinimapDisplayable.h"
#include "MLibrary.h"

void USmithMinimapTileEntry::NativeOnListItemObjectSet(UObject* listItemObject)
{
  if (TileImage == nullptr)
  {
    return;
  }
  
  if (listItemObject->Implements<UMinimapDisplayable>())
  {
    TileImage->SetBrushFromTexture(IMinimapDisplayable::Execute_GetMinimapDisplayTexture(listItemObject));
  }
}

void USmithMinimapTileEntry::SetTileTexture(UTexture2D* newTexture)
{
  if (TileImage != nullptr)
  {
    TileImage->SetBrushFromTexture(newTexture);
  }
}

