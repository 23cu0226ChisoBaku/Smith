// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithItemDescriptionWidget.h"
#include "Components/TextBlock.h"

USmithItemDescriptionWidget::USmithItemDescriptionWidget(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithItemDescriptionWidget::NativeConstruct()
{
  Super::NativeConstruct();
  
  ResetWidget();
}

void USmithItemDescriptionWidget::NativeDestruct()
{
  Super::NativeDestruct();
}

void USmithItemDescriptionWidget::SetItemInformation(const FString& ItemName, const FString& ItemDescription)
{
  if (ItemNameTextBlock != nullptr)
  {
    const FString nameStr = TEXT("◇"); 
    ItemNameTextBlock->SetText(FText::FromString(nameStr + ItemName));
  }

  if (ItemDescriptionTextBlock != nullptr)
  {
    ItemDescriptionTextBlock->SetText(FText::FromString(ItemDescription));
  }
}

void USmithItemDescriptionWidget::ResetWidget()
{
  if (ItemNameTextBlock != nullptr)
  {
    ItemNameTextBlock->SetText(FText());
  }

  if (ItemDescriptionTextBlock != nullptr)
  {
    ItemDescriptionTextBlock->SetText(FText());
  }
}