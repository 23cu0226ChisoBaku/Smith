// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithPlayerHP.h"

#include "Components/RichTextBlock.h"

void USmithPlayerHP::SetHP(int32 CurrentHP, int32 MaxHP)
{
  if (MaxHP != 0)
  {
    if (HPNumber != nullptr)
    {
      HPNumber->SetText(FText::FromString(FString::FromInt(CurrentHP) + TEXT("/") + FString::FromInt(MaxHP)));
    }

    const float newPercentage = StaticCast<float>(CurrentHP) / StaticCast<float>(MaxHP);
    percent = newPercentage;

    UpdateHP();
  }
}