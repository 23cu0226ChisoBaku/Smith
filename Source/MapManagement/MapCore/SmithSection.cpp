// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithSection.cpp

Author : MAI ZHICONG(バクチソウ)

Description : セクションクラス（マップの基本単位）

Update History: 2024/12/30 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithSection.h"
#include "SmithRect.h"
#include "SmithRoom.h"
#include "MLibrary.h"

namespace UE::Smith
{
  namespace Map
  {
    ///
    /// @brief FSmithSection実装
    ///
    // FSmithSection Implementation
    #pragma region FSmithSection Implementation
    class FSmithSection::SectionImpl
    {
      //---------------------------------------
      /*
                      ctorとdtor
      */
      //---------------------------------------        
      public:
        SectionImpl(uint8 sectionIdx)
          : m_sectionRect{}
          , m_room(nullptr)
          , m_roomLeft(0)
          , m_roomTop(0)
          , m_sectionIdx(sectionIdx)
        {
          
        }
        ~SectionImpl()
        {
          m_room.Reset();
        }

      public:
        void GenerateSection(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 defaultData)
        {
          const uint8 width = right - left + 1;
          const uint8 height = bottom - top + 1;
          m_sectionRect.GenerateRect(width, height, defaultData);
        }
        void GenerateRoom(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 data)
        {
          // 左上の始点座標がセクションを超えないように
          if (left >= m_sectionRect.GetWidth()
             || top >= m_sectionRect.GetHeight())
          {
            MDebug::LogError("Cannot Generate Room in Section " + FString::FromInt(m_sectionIdx));
            return;
          }   

          // 新しい部屋を作成
          TUniquePtr<FSmithRoom> roomTemp = ::MakeUnique<FSmithRoom>(m_sectionIdx);

          // 横と縦をセクションの最大幅に超えないように
          const uint8 width = ((right - left + 1) > m_sectionRect.GetWidth()) ? (m_sectionRect.GetWidth() - left) : (right - left + 1);
          const uint8 height = ((bottom - top + 1) > m_sectionRect.GetHeight()) ? (m_sectionRect.GetHeight() - top) : (bottom - top + 1);
          roomTemp->GenerateRoom(width, height, data);

          // 部屋があったら削除
          if (m_room.IsValid())
          {
            m_room.Reset();
          }

          // 作成した部屋をムーブ代入
          m_room = ::MoveTemp(roomTemp);
          m_roomLeft = left;
          m_roomTop = top;

          m_sectionRect.FillRect(left, top, right, bottom, data);
        }
        bool HasRoom() const
        {
          return m_room.IsValid() && m_room->GetWidth() > 1 && m_room->GetHeight() >= 1;
        }
        uint8 GetWidth() const
        {
          return m_sectionRect.GetWidth();
        }
        uint8 GetHeight() const
        {
          return m_sectionRect.GetHeight();
        }
        uint8 GetRoomWidth() const
        {
          if (!m_room.IsValid())
          {
            return 0;
          }
          else
          {
            return m_room->GetWidth();
          }
        }
        uint8 GetRoomHeight() const
        {
          if (!m_room.IsValid())
          {
            return 0;
          }
          else
          {
            return m_room->GetHeight();
          }
        }
        uint8 GetRoomLeft() const
        {
          return m_roomLeft;
        }
        uint8 GetRoomTop() const
        {
          return m_roomTop;
        }
        uint8 GetSectionIdx() const
        {
          return m_sectionIdx;
        }
        FSmithRect GetSectionRect() const
        {
          return m_sectionRect;
        }
        FSmithRect GetRoomRect() const
        {
          // 部屋がないと空っぽの矩形を返す
          if (!HasRoom())
          {
            return FSmithRect{};
          }
          else
          {
            return m_room->GetRoomRect();
          }
        }
      private:
        FSmithRect m_sectionRect;
        TUniquePtr<FSmithRoom> m_room;
        uint8 m_roomLeft;
        uint8 m_roomTop;
        uint8 m_sectionIdx;
    };
    #pragma endregion FSmithSection Implementation
    // end of FSmithSection Implementation

    // FSmithSection Interface
    #pragma region FSmithSection Interface
    FSmithSection::FSmithSection(uint8 sectionIdx)
      : m_pImpl(::MakeUnique<SectionImpl>(sectionIdx))
    { }
    FSmithSection::~FSmithSection()
    { 
      m_pImpl.Reset();
    }
    void FSmithSection::GenerateSection(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 defaultData)
    {
      m_pImpl->GenerateSection(left, top, right, bottom, defaultData);
    }
    void FSmithSection::GenerateRoom(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 data)
    {
      m_pImpl->GenerateRoom(left, top, right, bottom, data);
    }
    bool FSmithSection::HasRoom() const
    {
      return m_pImpl->HasRoom();
    }
    uint8 FSmithSection::GetWidth() const
    {
      return m_pImpl->GetWidth();
    }
    uint8 FSmithSection::GetHeight() const
    {
      return m_pImpl->GetHeight();
    }
    uint8 FSmithSection::GetRoomWidth() const
    {
      return m_pImpl->GetRoomWidth();
    }
    uint8 FSmithSection::GetRoomHeight() const
    {
      return m_pImpl->GetRoomHeight();
    }
    uint8 FSmithSection::GetRoomLeft() const
    {
      return m_pImpl->GetRoomLeft();
    }
    uint8 FSmithSection::GetRoomTop() const
    {
      return m_pImpl->GetRoomTop();
    }
    uint8 FSmithSection::GetSectionIdx() const
    {
      return m_pImpl->GetSectionIdx();
    }
    FSmithRect FSmithSection::GetSectionRect() const
    {
      return m_pImpl->GetSectionRect();
    }
    FSmithRect FSmithSection::GetRoomRect() const
    {
      return m_pImpl->GetRoomRect();
    }
    #pragma endregion FSmithSection Interface
    // end of FSmithSection Interface
  }
}
