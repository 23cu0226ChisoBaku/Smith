// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleLogModel.h"

#include "SmithBattleLogModelDefinition.h"

FSmithBattleLogModel FSmithBattleLogModel::CreateModel(USmithBattleLogModelDefinition* LogDefinition)
{
  check(LogDefinition != nullptr);

  FSmithBattleLogModel model;
  model.m_definition = LogDefinition;

  return model;
}

FString FSmithBattleLogModel::GetName() const
{
  return IsValid() ? m_definition->LogData.Name : TEXT("");
}

EBattleLogModelType FSmithBattleLogModel::GetType() const
{
  return IsValid() ? m_definition->LogData.Type : EBattleLogModelType::ModelTypeCount;
}

bool FSmithBattleLogModel::IsValid() const
{
  return (m_definition != nullptr) && (m_definition->LogClass != nullptr);
}


