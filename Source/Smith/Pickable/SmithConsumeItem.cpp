// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithConsumeItem.h"

USmithConsumeItem::USmithConsumeItem(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithConsumeItem::BeginDestroy()
{
  Super::BeginDestroy();
}

void USmithConsumeItem::Use(IItemUseable* user)
{
  useImpl(user);
}

