// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUIInventoryEntry.h"
#include "Components/RichTextBlock.h"
#include "Components/Image.h"
#include "ISmithItemWidgetParameterizable.h"

USmithUIInventoryEntry::USmithUIInventoryEntry(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{

}
void USmithUIInventoryEntry::NativeOnListItemObjectSet(UObject* listItemObject)
{
  if (ItemNameLabel == nullptr || ItemIconImage == nullptr || SelectArrow == nullptr)
  { 
    return;
  }

  ISmithItemWidgetParameterizable* itemParam = Cast<ISmithItemWidgetParameterizable>(listItemObject);
  if (itemParam != nullptr)
  {
    ItemNameLabel->SetText(FText::FromString(itemParam->GetName()));
    LabelBackground->SetBrushTintColor(itemParam->GetLabelColor());
    ItemIconImage->SetBrushFromTexture(itemParam->GetIconImage());
    SelectArrow->SetVisibility(ESlateVisibility::Hidden);
  }
}

void USmithUIInventoryEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
  if (SelectArrow == nullptr)
  {
    return;
  }

  if (bIsSelected)
  {
    SelectArrow->SetVisibility(ESlateVisibility::Visible);
  }
  else
  {
    SelectArrow->SetVisibility(ESlateVisibility::Hidden);
  }
}

