// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapModel.h"

FSmithMapModel::FSmithMapModel()
  : m_actorClass(nullptr)
  , m_sizeX(0u)
  , m_sizeY(0u)
  , m_type(EMapModelType::TypeCount)
{ }

FSmithMapModel FSmithMapModel::CreateModel(UClass* ActorClass, uint8 SizeX, uint8 SizeY, EMapModelType MapModelType)
{
  check(ActorClass != nullptr);

  FSmithMapModel model;
  model.m_actorClass = ActorClass;
  model.m_sizeX = SizeX;
  model.m_sizeY = SizeY;
  model.m_type = MapModelType;

  return model;
}

bool FSmithMapModel::IsValid() const
{
  if (   (m_actorClass == nullptr)
      || (m_sizeX == 0u)
      || (m_sizeY == 0u)
      || (StaticCast<uint8>(m_type) >= ((uint8)EMapModelType::TypeCount))
     )
  {
    return false;
  }

  return true;
}