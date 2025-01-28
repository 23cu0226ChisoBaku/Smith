// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPickable.h"
#include "ICanPick.h"

USmithPickable::USmithPickable(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithPickable::BeginDestroy()
{
  Super::BeginDestroy();
}

void USmithPickable::OnPick(ICanPick* picker)
{
  onPickImpl(picker);
}

FString USmithPickable::GetPickType() const
{
  return PickableTypeName;
}