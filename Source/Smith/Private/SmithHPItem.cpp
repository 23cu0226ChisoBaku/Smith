// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithHPItem.h"
#include "IItemUseable.h"
#include "MLibrary.h"

USmithHPItem::USmithHPItem(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{

}

void USmithHPItem::BeginDestroy()
{
  Super::BeginDestroy();
}

void USmithHPItem::useImpl(IItemUseable* user)
{
  if (user == nullptr)
  {
    return;
  }

  user->UseItem(this);
}

void USmithHPItem::SetRecoveryPercentage(double percentage)
{
  RecoveryPercentage = percentage;
}

double USmithHPItem::GetRecoveryPercentage() const
{
  return RecoveryPercentage;
}
