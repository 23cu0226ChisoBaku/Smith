// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithAIStrategy.h

Author : MAI ZHICONG(バクチソウ)

Description : Smith専用簡易的なAI（Behaviour Treeにする予定あり）

Update History: 2025/01/14 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithAIStrategy.h"

USmithAIStrategy::USmithAIStrategy(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_owner(nullptr)
{ }

void USmithAIStrategy::BeginDestroy()
{
  m_owner == nullptr;
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
  return ::IsValid(m_owner) ? m_owner : nullptr;
}