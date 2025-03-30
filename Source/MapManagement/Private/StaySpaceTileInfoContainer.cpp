// Fill out your copyright notice in the Description page of Project Settings.
/*

StaySpaceTileInfoContainer.cpp

Author : MAI ZHICONG(バクチソウ)

Description : 

Update History: 2025/01/05 作成
                2025/02/04 インターフェース修正   SetPickable、GetPickable、CanPlacePickable削除
                                                SetEvent、GetEvent追加
Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "StaySpaceTileInfoContainer.h"

#include "UObject/WeakInterfacePtr.h"
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
        void SetMapObj(AActor* mapObj)
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
        AActor* GetMapObject() const
        {
          return m_mapObj.IsValid() ? m_mapObj.Get() : nullptr;
        }
        ISmithMapEvent* GetEvent() const
        {
          return m_event.IsValid() ? m_event.Get() : nullptr;
        }
        void ResetContainer()
        {
          m_mapObj.Reset();
          m_event.Reset();
        }

      private:
        TWeakObjectPtr<AActor> m_mapObj;
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

    void FStaySpaceTileInfoContainer::SetMapObj(AActor* mapObj)
    {
      m_pImpl->SetMapObj(mapObj);
    }

    void FStaySpaceTileInfoContainer::SetEvent(ISmithMapEvent* event)
    {
      m_pImpl->SetEvent(event);
    }

    AActor* FStaySpaceTileInfoContainer::GetMapObject() const
    {
      return m_pImpl->GetMapObject();
    }

    ISmithMapEvent* FStaySpaceTileInfoContainer::GetEvent() const
    {
      return m_pImpl->GetEvent();
    }

    void FStaySpaceTileInfoContainer::resetImpl()
    {
      m_pImpl->ResetContainer();
    }

    bool FStaySpaceTileInfoContainer::isAbleToStayOnImpl() const
    {
      return m_pImpl->IsAbleToStayOn();
    }
  }
}
