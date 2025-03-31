// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithPickable.h"

USmithPickable::USmithPickable(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithPickable::BeginDestroy()
{
  Super::BeginDestroy();
}

FString USmithPickable::GetPickType() const
{
  return PickableTypeName;
}