// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CurrentLevel.h"
#include "Components/RichTextBlock.h"

void UUI_CurrentLevel::NativeConstruct()
{
  Super::NativeConstruct();
  if(CurrentLevel != nullptr)
  {
    CurrentLevel->SetText(FText::FromString(TEXT("1F")));
  }
}

void UUI_CurrentLevel::SetLevel(int32 level)
{
  if(CurrentLevel != nullptr)
  {
    CurrentLevel->SetText(FText::FromString(FString::FromInt(level) + "F"));
  }
}
