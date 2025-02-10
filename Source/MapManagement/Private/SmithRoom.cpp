// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithRoom.cpp

Author : MAI ZHICONG(バクチソウ)

Description : Smith部屋（不思議ダンジョン風）

Update History: 2024/12/29 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithRoom.h"
#include "SmithRect.h"
#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief FSmithRoom
    ///
    // FSmithRoom Implementation
    #pragma region FSmithRoom Implementation
    class FSmithRoom::RoomImpl
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------  
      public:
        RoomImpl(uint8 sectionIdx)
          : m_roomRect{}
          , m_sectionIdx(sectionIdx)
        { }
        ~RoomImpl()
        { }

      public:
        void GenerateRoom(uint8 width, uint8 height, uint8 data)
        {
          m_roomRect.GenerateRect(width, height, data);
        }
        uint8 GetWidth() const
        {
          return m_roomRect.GetWidth();
        }
        uint8 GetHeight() const
        {
          return m_roomRect.GetHeight();
        }
        uint8 GetSectionIdx() const
        {
          return m_sectionIdx;
        }
        FSmithRect GetRect() const
        {
          return m_roomRect;
        }
      private:
        FSmithRect m_roomRect;
        uint8 m_sectionIdx;
    };
    #pragma endregion FSmithRoom Implementation
    // end of FSmithRoom Implementation

    // FSmithRoom Interface
    #pragma region FSmithRoom Interface
    FSmithRoom::FSmithRoom(uint8 sectionIdx)
      : m_pImpl(::MakeUnique<RoomImpl>(sectionIdx))
    { }
    FSmithRoom::~FSmithRoom()
    {
      m_pImpl.Reset();
    }
    void FSmithRoom::GenerateRoom(uint8 width, uint8 height, uint8 data)
    {
      m_pImpl->GenerateRoom(width, height, data);
    }
    uint8 FSmithRoom::GetWidth() const
    {
      return m_pImpl->GetWidth();
    }
    uint8 FSmithRoom::GetHeight() const
    {
      return m_pImpl->GetHeight();
    }
    uint8 FSmithRoom::GetSectionIdx() const
    {
      return m_pImpl->GetSectionIdx();
    }
    FSmithRect FSmithRoom::GetRoomRect() const
    {
      return m_pImpl->GetRect();
    }
    #pragma endregion FSmithRoom Interface
    // end of FSmithRoom Interface
  }
}

