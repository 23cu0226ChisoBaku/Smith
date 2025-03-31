// Fill out your copyright notice in the Description page of Project Settings.
/*

ObstacleInfoContainer.cpp

Author : MAI ZHICONG(バクチソウ)

Description : 障害物タイルの情報コンテナ

Update History: 2025/01/05 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "ObstacleTileInfoContainer.h"

namespace UE::Smith
{
  namespace Map
  {
    FObstacleTileInfoContainer::FObstacleTileInfoContainer()
      : FTileInfoContainer(ETileType::Wall)
    { }

    FObstacleTileInfoContainer::~FObstacleTileInfoContainer()
    { }

    FObstacleTileInfoContainer::FObstacleTileInfoContainer(const FObstacleTileInfoContainer& other)
      : FTileInfoContainer(other)
    { }

    FObstacleTileInfoContainer& FObstacleTileInfoContainer::operator=(const FObstacleTileInfoContainer& other)
    {
      if (this != &other)
      {
        FTileInfoContainer::operator=(other);
      }
      return *this;
    }

    FObstacleTileInfoContainer::FObstacleTileInfoContainer(FObstacleTileInfoContainer&& other) noexcept
      : FTileInfoContainer(::MoveTemp(other))
    { }

    FObstacleTileInfoContainer& FObstacleTileInfoContainer::operator=(FObstacleTileInfoContainer&& other) noexcept
    {
      if (this != &other)
      {
        FTileInfoContainer::operator=(::MoveTemp(other));
      }

      return *this;
    }

    bool FObstacleTileInfoContainer::isAbleToStayOnImpl() const
    {
      return false;
    }
  }
}
