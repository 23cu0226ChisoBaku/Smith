// Fill out your copyright notice in the Description page of Project Settings.


#include "TileInfoContainer.h"

namespace UE::Smith
{
  namespace Map
  {
    FTileInfoContainer::FTileInfoContainer(ETileType type)
      : m_tileType(type) 
    { }

    FTileInfoContainer::FTileInfoContainer(const FTileInfoContainer& other) noexcept
      : m_tileType(other.m_tileType)
    { }
    
    FTileInfoContainer& FTileInfoContainer::operator=(const FTileInfoContainer& other) noexcept
    {
      if (this != &other)
      {
        m_tileType = other.m_tileType;
      }

      return *this;
    }

    FTileInfoContainer::FTileInfoContainer(FTileInfoContainer&& other) noexcept
      : m_tileType(other.m_tileType)
    { }

    FTileInfoContainer& FTileInfoContainer::operator=(FTileInfoContainer&& other) noexcept
    {
      if (this != &other)
      {
        m_tileType = ::MoveTemp(other.m_tileType);
        other.m_tileType = StaticCast<ETileType>(0);
      }

      return *this;
    }

    bool FTileInfoContainer::IsAbleToStayOn() const
    {
      return isAbleToStayOn_Impl();
    }

    bool FTileInfoContainer::isAbleToStayOn_Impl() const
    {
      return true;
    }

    void FTileInfoContainer::Reset()
    {
      reset_Impl();
    }

    void FTileInfoContainer::reset_Impl()
    {
      
    }
  }
}
