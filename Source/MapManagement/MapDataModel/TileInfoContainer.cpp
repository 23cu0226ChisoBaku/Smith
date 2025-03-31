// Fill out your copyright notice in the Description page of Project Settings.
/*

TileInfoContainer.cpp

Author : MAI ZHICONG(バクチソウ)

Description : マップタイルの情報コンテナ実装

Update History: 2025/01/05 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "TileInfoContainer.h"

namespace UE::Smith
{
  namespace Map
  {
    FTileInfoContainer::FTileInfoContainer(ETileType type)
      : m_tileType(type) 
      , m_tileActor(nullptr)
    { }

    FTileInfoContainer::FTileInfoContainer(const FTileInfoContainer& other) noexcept
      : m_tileType(other.m_tileType)
      , m_tileActor(other.m_tileActor)
    { }
    
    FTileInfoContainer& FTileInfoContainer::operator=(const FTileInfoContainer& other) noexcept
    {
      if (this != &other)
      {
        m_tileType = other.m_tileType;
        m_tileActor = other.m_tileActor;
      }

      return *this;
    }

    FTileInfoContainer::FTileInfoContainer(FTileInfoContainer&& other) noexcept
      : m_tileType(::MoveTemp(other.m_tileType))
      , m_tileActor(::MoveTemp(other.m_tileActor))
    { }

    FTileInfoContainer& FTileInfoContainer::operator=(FTileInfoContainer&& other) noexcept
    {
      if (this != &other)
      {
        m_tileType = ::MoveTemp(other.m_tileType);
        m_tileActor = ::MoveTemp(other.m_tileActor);
        other.m_tileType = StaticCast<ETileType>(0);
        other.m_tileActor = nullptr;
      }

      return *this;
    }

    bool FTileInfoContainer::IsAbleToStayOn() const
    {
      return isAbleToStayOnImpl();
    }

    void FTileInfoContainer::Reset()
    {
      resetImpl();
    }

    bool FTileInfoContainer::isAbleToStayOnImpl() const
    {
      return true;
    }

    void FTileInfoContainer::resetImpl()
    { }
  }
}
