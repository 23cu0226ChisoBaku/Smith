// Fill out your copyright notice in the Description page of Project Settings.
/*

StaySpaceTileInfoContainer.cpp

Author : MAI ZHICONG

Description : 

Update History: 2025/01/05 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "StaySpaceTileInfoContainer.h"
#include "UObject/WeakInterfacePtr.h"
#include "ICanSetOnMap.h"
#include "ISmithMapEvent.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief FStaySpaceTileInfoContainer実装クラス
    ///
    class FStaySpaceTileInfoContainer::StaySpaceTileInfoImpl
    {
      public:
        StaySpaceTileInfoImpl()
          : m_mapObj(nullptr)
          , m_event(nullptr)
        { }
      public:
        void SetMapObj(ICanSetOnMap* mapObj)
        {
          m_mapObj = mapObj;
        }
        void SetEvent(ISmithMapEvent* event)
        {
          m_event = event;
        }
        bool IsAbleToStayOn() const
        {
          return !m_mapObj.IsValid();
        }
        ICanSetOnMap* GetMapObject() const
        {
          return m_mapObj.Get();
        }
        ISmithMapEvent* GetEvent() const
        {
          return m_event.Get();
        }
        void ResetContainer()
        {
          m_mapObj.Reset();
        }

      private:
        TWeakInterfacePtr<ICanSetOnMap> m_mapObj;
        TWeakInterfacePtr<ISmithMapEvent> m_event;
    };

    FStaySpaceTileInfoContainer::FStaySpaceTileInfoContainer(ETileType type)
      : FTileInfoContainer(type)
      , m_pImpl(::MakeUnique<StaySpaceTileInfoImpl>())
    { }

    FStaySpaceTileInfoContainer::~FStaySpaceTileInfoContainer()
    {
      m_pImpl.Reset();
    }

    FStaySpaceTileInfoContainer::FStaySpaceTileInfoContainer(const FStaySpaceTileInfoContainer& other)
      : FTileInfoContainer(other)
      , m_pImpl(::MakeUnique<StaySpaceTileInfoImpl>(*other.m_pImpl))
    { }

    FStaySpaceTileInfoContainer& FStaySpaceTileInfoContainer::operator=(const FStaySpaceTileInfoContainer& other)
    {
      if (this != &other)
      {
        FTileInfoContainer::operator=(other);
        // 例外安全
        TUniquePtr<StaySpaceTileInfoImpl> temp = ::MakeUnique<StaySpaceTileInfoImpl>(*other.m_pImpl);

        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(temp);
      }

      return *this;
    }

    FStaySpaceTileInfoContainer::FStaySpaceTileInfoContainer(FStaySpaceTileInfoContainer&& other) noexcept
      : FTileInfoContainer(other)
      , m_pImpl(::MoveTemp(other.m_pImpl))
    { }

    FStaySpaceTileInfoContainer& FStaySpaceTileInfoContainer::operator=(FStaySpaceTileInfoContainer&& other) noexcept
    {
      if (this != &other)
      {
        FTileInfoContainer::operator=(::MoveTemp(other));

        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(other.m_pImpl);
      }

      return *this;
    }

    void FStaySpaceTileInfoContainer::SetMapObj(ICanSetOnMap* mapObj)
    {
      m_pImpl->SetMapObj(mapObj);
    }

    // TODO not implemented
    void FStaySpaceTileInfoContainer::SetPickable(IPickable* pickable)
    {
      unimplemented()
    }
    void FStaySpaceTileInfoContainer::SetEvent(ISmithMapEvent* event)
    {
      m_pImpl->SetEvent(event);
    }

    ICanSetOnMap* FStaySpaceTileInfoContainer::GetMapObject() const
    {
      return m_pImpl->GetMapObject();
    }

    ISmithMapEvent* FStaySpaceTileInfoContainer::GetEvent() const
    {
      return m_pImpl->GetEvent();
    }

    // TODO not implemented
    bool FStaySpaceTileInfoContainer::CanPlacePickable() const
    {
      unimplemented();
      return false;
    }

    void FStaySpaceTileInfoContainer::reset_Impl()
    {
      m_pImpl->ResetContainer();
    }

    bool FStaySpaceTileInfoContainer::isAbleToStayOn_Impl() const
    {
      return m_pImpl->IsAbleToStayOn();
    }
  }
}
