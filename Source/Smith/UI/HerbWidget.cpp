// Fill out your copyright notice in the Description page of Project Settings.


#include "HerbWidget.h"
#include "Components/RichTextBlock.h"

void UHerbWidget::NativeConstruct()
{
  Super::NativeConstruct();
  if(HerbNum != nullptr)
  {
    HerbNum->SetText(FText::FromString(FString::FromInt(0)));
  }
}

void UHerbWidget::SetNum(int32 setNum)
{
  if (setNum < 0)
  {
    return;
  }
  
  if(HerbNum != nullptr)
  {
    FString num = FString::FromInt(setNum);
    if(setNum < 10)
    {
      num = TEXT("0" + FString::FromInt(setNum));
    }
    HerbNum->SetText(FText::FromString(num));
  }
}