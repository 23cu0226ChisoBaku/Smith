// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithAIStrategy.h"

USmithAIStrategy::USmithAIStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_owner(nullptr)
{ }

void USmithAIStrategy::BeginDestroy()
{
  Super::BeginDestroy();
}

bool USmithAIStrategy::Execute()
{
  return executeImpl();
}

void USmithAIStrategy::SetOwner(AActor* owner)
{
  m_owner = owner;
}

AActor* USmithAIStrategy::GetOwner() const
{
  return m_owner;
}