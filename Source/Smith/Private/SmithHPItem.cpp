// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithHPItem.h"
#include "MLibrary.h"

USmithHPItem::USmithHPItem(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{

}

void USmithHPItem::BeginDestroy()
{
  Super::BeginDestroy();

  MDebug::LogWarning("HP Item Destroyed");
}

void USmithHPItem::useImpl()
{
  MDebug::Log("Use HP ITem");
}