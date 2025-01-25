// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithItemDescriptionWidget.h"
#include "Components/TextBlock.h"

USmithItemDescriptionWidget::USmithItemDescriptionWidget(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithItemDescriptionWidget::NativeConstruct()
{
  Super::NativeConstruct();
  
  if (ItemNameTextBlock != nullptr)
  {
    ItemNameTextBlock->SetText(FText::FromString(TEXT("アイテム名")));
  }

  if (ItemDescriptionTextBlock != nullptr)
  {
    ItemNameTextBlock->SetText(FText::FromString(TEXT("アイテム説明")));
  }
}

void USmithItemDescriptionWidget::NativeDestruct()
{

}

void USmithItemDescriptionWidget::SetItemInformation(const FString& ItemName, const FString& ItemDescription)
{
  if (ItemNameTextBlock != nullptr)
  {
    ItemNameTextBlock->SetText(FText::FromString(ItemName));
  }

  if (ItemDescriptionTextBlock != nullptr)
  {
    ItemDescriptionTextBlock->SetText(FText::FromString(ItemDescription));
  }
}