// Fill out your copyright notice in the Description page of Project Settings.
/*

SmithRect.cpp

Author : MAI ZHICONG(バクチソウ)

Description : MapManagement モジュール専用矩形

Update History: 2024/12/29 作成
								2025/01/30 Description修正

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithRect.h"

#include "DataStructure/Dimension2Array.h"

namespace UE::FSmithRect::Private
{
  // 矩形の範囲外を超える時のデフォルト値
  // 255U
  constexpr uint8 OUT_OF_BOUNDS = 0xffu;
}

namespace UE::Smith
{
	namespace Map
	{
    ///
    /// @brief FSmithRect実装
    ///
    // SmithRect Implementation
    #pragma region SmithRect Implementation
    class FSmithRect::RectImpl
    {
      //---------------------------------------
      /*
                　　　　エイリアス
      */
      //---------------------------------------
      private:
        using Dimension2D_uint8 = typename UE::MLibrary::MDataStructure::TDimension2Array<uint8>;
        using Dimension2D_uint8_handle = typename UE::MLibrary::MDataStructure::TDimension2ArrayHandle<uint8>;

      //---------------------------------------
      /*
                      ctorとdtor
      */
      //--------------------------------------- 
      public:
        RectImpl()
          : m_rect{}
        { }
        ~RectImpl()
        { }

        //---------------------------------------
        /*
                        コピー
        */
        //---------------------------------------
        RectImpl(const RectImpl& other)
          : m_rect(other.m_rect)
        { }
        RectImpl& operator=(const RectImpl& other)
        {
          if (this != &other)
          {
            m_rect = other.m_rect;
          }
          return *this;
        }

      public:
        void GenerateRect(uint8 width, uint8 height, uint8 defaultData)
        {
          Dimension2D_uint8 temp(StaticCast<uint64>(height), StaticCast<uint64>(width));
          for (uint64 y = 0; y < temp.Row(); ++y)
          {
            for (uint64 x = 0; x < temp.Column(); ++x)
            {
              temp.At(y, x) = defaultData;
            }
          }

          m_rect = ::MoveTemp(temp);
        }

        void SetRect(uint8 x, uint8 y, uint8 value)
        {
          if (IsOutOfBounds(x, y))
          {
            return;
          }

          m_rect.At(y, x) = value;
        }

        void FillRect(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 data)
        {
          if (IsOutOfBounds(left, top))
          {
            return;
          }

          // 入力はそれぞれの最大値を超えたら最大値にする
          // 右端（横幅）
          const uint8 fillRight = (right + 1) > m_rect.Column() ? StaticCast<uint8>(m_rect.Column()) : (right + 1);
          // 下端（縦幅）
          const uint8 fillBottom = (bottom + 1) > m_rect.Row() ? StaticCast<uint8>(m_rect.Row()) : (bottom + 1);
          for (uint8 y = top; y < fillBottom; ++y)
          {
            for (uint8 x = left; x < fillRight; ++x)
            {
              m_rect.At(y, x) = data;
            }
          }
        }

        uint8 GetWidth() const
        {
          return StaticCast<uint8>(m_rect.Column());
        }

        uint8 GetHeight() const
        {
          return StaticCast<uint8>(m_rect.Row());
        }

        uint8 GetRect(uint8 x, uint8 y)
        {
          using namespace UE::FSmithRect::Private;

          // 範囲外だったらOUT_OF_BOUNDSデータを返す(255)
          if (IsOutOfBounds(x, y))
          {
            return OUT_OF_BOUNDS;
          }

          return m_rect.At_ReadOnly(y, x);
        }

        bool IsOutOfBounds(uint8 x, uint8 y) const
        {
          return (x >= GetWidth()) || (y >= GetHeight());
        }
        
      #pragma endregion SmithRect Implementation
      // end of SmithRect Implementation
      private:
        // 矩形の元データ（二次元配列）
        Dimension2D_uint8 m_rect;
    };

    FSmithRect::FSmithRect()
      : m_pImpl(::MakeUnique<RectImpl>())
    { }

    FSmithRect::~FSmithRect()
    {
      m_pImpl.Reset();
    }

    FSmithRect::FSmithRect(const FSmithRect& other)
      : m_pImpl(::MakeUnique<RectImpl>(*other.m_pImpl))
    { }

    FSmithRect& FSmithRect::operator=(const FSmithRect& other)
    {
      if (this != &other)
      {
        // 例外安全
        TUniquePtr<RectImpl> implTemp = ::MakeUnique<RectImpl>(*other.m_pImpl);

        m_pImpl.Reset();
        // ユニークポインターのためインスタンスを使用してコピーする
        m_pImpl = ::MoveTemp(implTemp);
      }

      return *this;
    }

    FSmithRect::FSmithRect(FSmithRect&& other) noexcept
      : m_pImpl(::MoveTemp(other.m_pImpl))
    { }

    FSmithRect& FSmithRect::operator=(FSmithRect&& other) noexcept
    {
      if (this != &other)
      {
        m_pImpl.Reset();
        m_pImpl = ::MoveTemp(other.m_pImpl);
      }

      return *this;
    }

    void FSmithRect::GenerateRect(uint8 width, uint8 height, uint8 defaultData)
    {
      m_pImpl->GenerateRect(width, height, defaultData);
    }

    void FSmithRect::SetRect(uint8 x, uint8 y, uint8 data)
    {
      m_pImpl->SetRect(x, y, data);
    }

    void FSmithRect::FillRect(uint8 left, uint8 top, uint8 right, uint8 bottom, uint8 data)
    {
      m_pImpl->FillRect(left, top, right, bottom, data);
    }

    uint8 FSmithRect::GetWidth() const
    {
      return m_pImpl->GetWidth();
    }

    uint8 FSmithRect::GetHeight() const
    {
      return m_pImpl->GetHeight();
    }

    uint8 FSmithRect::GetRect(uint8 x, uint8 y) const
    {
      return m_pImpl->GetRect(x, y);
    }

    bool FSmithRect::IsOutOfBounds(uint8 x, uint8 y) const
    {
      return m_pImpl->IsOutOfBounds(x, y);
    }
  }
}
