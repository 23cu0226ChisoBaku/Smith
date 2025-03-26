// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithUpgradeMaterialPreview.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USmithUpgradeMaterialPreview::NativeConstruct()
{
  Super::NativeConstruct();
  ResetWidget();
}

void USmithUpgradeMaterialPreview::SetMaterialImage(UTexture2D* texture)
{
  if (MaterialImage != nullptr)
  {
    MaterialImage->SetBrushFromTexture(texture);
    MaterialImage->SetVisibility(ESlateVisibility::Visible);
  }
}

void USmithUpgradeMaterialPreview::SetMaterialNameLabel(const FString& name)
{
  if (MaterialNameLabel != nullptr)
  {
    MaterialNameLabel->SetText(FText::FromString(name));
    MaterialNameLabel->SetVisibility(ESlateVisibility::Visible);
  }
}

void USmithUpgradeMaterialPreview::SetNamePlateImageColor(const FColor& color)
{
  if (MaterialNamePlateImage != nullptr)
  {
    MaterialNamePlateImage->SetBrushTintColor(color);
    MaterialNamePlateImage->SetVisibility(ESlateVisibility::Visible);
  }
}

void USmithUpgradeMaterialPreview::ResetWidget()
{
  if (MaterialImage != nullptr)
  {
    MaterialImage->SetVisibility(ESlateVisibility::Hidden);
  }

  if (MaterialNameLabel != nullptr)
  {
    MaterialNameLabel->SetVisibility(ESlateVisibility::Hidden);
  }

  if (MaterialNamePlateImage != nullptr)
  {
    MaterialNamePlateImage->SetVisibility(ESlateVisibility::Hidden);
  }
}