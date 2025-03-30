// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapModel.h"

#include "SmithMapModelDefinition.h"

FSmithMapModel FSmithMapModel::CreateModel(USmithMapModelDefinition* Definition)
{
  FSmithMapModel model;
  model.m_definition = Definition;

  return model;
}

uint8 FSmithMapModel::GetSizeX() const
{
  return IsValid() ? m_definition->MapObjectDataInstance.MapSizeX : 0u;
}

uint8 FSmithMapModel::GetSizeY() const
{
  return IsValid() ? m_definition->MapObjectDataInstance.MapSizeY : 0u;
}

EMapModelType FSmithMapModel::GetModelType() const
{
  return IsValid() ? m_definition->MapObjectDataInstance.MapModelType : EMapModelType::TypeCount;
}

bool FSmithMapModel::IsValid() const
{
  if (!m_definition.IsValid())
  {
    return false;
  }

  if (m_definition->MapObjectActorClass == nullptr)
  {
    return false;
  }

  if (  (m_definition->MapObjectDataInstance.MapSizeX == 0u) 
      ||(m_definition->MapObjectDataInstance.MapSizeY == 0u)
     )
  {
    return false;
  }

  return true;
}