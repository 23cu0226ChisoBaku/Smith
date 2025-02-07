// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTestAIAttackStrategy.h"
#include "MLibrary.h"

USmithTestAIAttackStrategy::USmithTestAIAttackStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

void USmithTestAIAttackStrategy::BeginDestroy()
{
  Super::BeginDestroy();
}

bool USmithTestAIAttackStrategy::executeImpl()
{
  MDebug::Log(FString::FromInt(m_attackFormatTables.Num()));
  return true;
}