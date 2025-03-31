// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithEventModel.h"

#include "SmithEventModelDefinition.h"

FSmithEventModel FSmithEventModel::CreateModel(USmithEventModelDefinition* DefinitionAsset)
{
  FSmithEventModel model;
  model.m_definition = DefinitionAsset;

  return model;
}

FString FSmithEventModel::GetSucceededMsg() const
{
  return IsValid() ? m_definition->EventData.SucceededMessage : TEXT("イベント実行成功");
}

FString FSmithEventModel::GetFailedMsg() const
{
  return IsValid() ? m_definition->EventData.FailedMessage : TEXT("イベント実行失敗");
}

bool FSmithEventModel::IsValid() const
{
  return (m_definition != nullptr) && (m_definition->EventClass != nullptr);
}