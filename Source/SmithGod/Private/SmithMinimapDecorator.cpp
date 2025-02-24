// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMinimapDecorator.h"

uint8 USmithMinimapDecorator::ConvertToTileType(EMapObjType mapObjType)
{
  switch (mapObjType)
  {
    case EMapObjType::Player:
    {
      return Player;
    }
    case EMapObjType::Enemy:
    {
      return Enemy;
    }
    case EMapObjType::Pickable:
    {
      return Pickable;
    }
    case EMapObjType::NextLevel:
    {
      return NextLevel;
    }
    default:
    {
      return Invalid;
    }
  }
}

uint8 USmithMinimapDecorator::ConvertToTileType(ETileType tileType)
{
  switch (tileType)
  {
    case ETileType::Void:
    {
      return Void;
    }
    case ETileType::Ground:
    {
      return Ground;
    }
    case ETileType::Wall:
    {
      return Wall;
    }
    case ETileType::Corridor:
    {
      return Corridor;
    }
    default:
    {
      return Invalid;
    }
  }
}

void USmithMinimapDecorator::AssignDecorator(IMinimapDecorator* decorator)
{
  m_minimapDecorator.SetInterface(decorator);
}

void USmithMinimapDecorator::UpdateTile(uint8 tileData, uint8 x, uint8 y, UTexture2D* tileTexture)
{
  if (m_minimapDecorator != nullptr)
  {
    m_minimapDecorator->UpdateTile(tileData, x, y, tileTexture);
  }
}

void USmithMinimapDecorator::PostInitProperties()
{
  Super::PostInitProperties();

  BeginPlay();
}

void USmithMinimapDecorator::BeginPlay()
{
  
}