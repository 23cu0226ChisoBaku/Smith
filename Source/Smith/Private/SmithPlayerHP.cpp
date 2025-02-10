// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithPlayerHP.h"
#include "Components/RichTextBlock.h"
#include "MLibrary.h"

void USmithPlayerHP::SetHP(float _percent)
{
  percent = _percent;
  UpdateHP();
}

void USmithPlayerHP::SetHPNumber(int32 maxHp,int32 currentHp)
{
  HPNumber->SetText(FText::FromString(FString::FromInt(currentHp) + TEXT("/") + FString::FromInt(maxHp)));
}