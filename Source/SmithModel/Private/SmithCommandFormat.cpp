// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithCommandFormat.h"
#include "FormatType.h"

using namespace UE::MLibrary::MDataStructure;

namespace UE::Smith
{
  namespace Battle
  {
    FSmithCommandFormat::FSmithCommandFormat()
      : m_formatArr()
      , m_cnCoord({})
    {

    }

    FSmithCommandFormat::FSmithCommandFormat(const ESmithFormatType* srcData, size_t dataCnt, uint64 row, uint64 column)
      : m_formatArr(TDimension2Array<ESmithFormatType>(srcData, dataCnt, row, column))
      , m_cnCoord({})
    { 
      setCNCoord();
    }

    FSmithCommandFormat::~FSmithCommandFormat()
    { }

    FSmithCommandFormat::FSmithCommandFormat(const FSmithCommandFormat& other)
      : m_formatArr(other.m_formatArr)
      , m_cnCoord(other.m_cnCoord)
    { }

    FSmithCommandFormat& FSmithCommandFormat::operator=(const FSmithCommandFormat& other)
    {
      if (this != &other)
      {
        m_formatArr = other.m_formatArr;
        m_cnCoord = other.m_cnCoord;
      }

      return *this;
    }

    FSmithCommandFormat::FSmithCommandFormat(FSmithCommandFormat&& other) noexcept
      : m_formatArr(::MoveTemp(other.m_formatArr))
      , m_cnCoord(::MoveTemp(other.m_cnCoord))
    {
      other.m_formatArr = TDimension2Array<ESmithFormatType>::EmptyArray;
    }

    FSmithCommandFormat& FSmithCommandFormat::operator=(FSmithCommandFormat&& other) noexcept
    {
      if (this != &other)
      {
        this->m_formatArr = ::MoveTemp(other.m_formatArr);
        this->m_cnCoord = other.m_cnCoord;

        other.m_formatArr = TDimension2Array<ESmithFormatType>::EmptyArray;
      }

      return *this;
    }
    void FSmithCommandFormat::SetupFormat(const ESmithFormatType* srcData, size_t dataCnt, uint64 row, uint64 column)
    {
      check(srcData != nullptr)
      check(dataCnt == row * column)

      m_formatArr = TDimension2Array<ESmithFormatType>(srcData, dataCnt, row, column);
      setCNCoord();
    }

    void FSmithCommandFormat::setCNCoord()
    {
      for (uint64 y = 0; y < GetRow(); ++y)
      {
        for (uint64 x = 0; x < GetColumn(); ++x)
        {
          auto formatType = m_formatArr.At_ReadOnly(y, x);
          if ((formatType == ESmithFormatType::CENTER_NO_EFFECT) || (formatType == ESmithFormatType::CENTER_EFFECT))
          {
            m_cnCoord.x = StaticCast<uint8>(x);
            m_cnCoord.y = StaticCast<uint8>(y);

            return;
          }
        }
      }
    }

    ESmithFormatType FSmithCommandFormat::GetFormatData(uint64 x, uint64 y) const
    {
      return m_formatArr.At_ReadOnly(y, x);
    }
  }
}
