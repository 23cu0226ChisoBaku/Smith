// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithCommandFormat.h"

using namespace UE::MLibrary::MDataStructure;

namespace UE::Smith
{
  inline namespace Battle
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
    {
      this->m_formatArr = other.m_formatArr;
      this->m_cnCoord = other.m_cnCoord;
    }

    FSmithCommandFormat& FSmithCommandFormat::operator=(const FSmithCommandFormat& other)
    {
      if (this != &other)
      {
        this->m_formatArr = other.m_formatArr;
        this->m_cnCoord = other.m_cnCoord;
      }

      return *this;
    }

    FSmithCommandFormat::FSmithCommandFormat(FSmithCommandFormat&& other) noexcept
    {
      this->m_formatArr = ::MoveTemp(other.m_formatArr);
      this->m_cnCoord = other.m_cnCoord;

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
    void FSmithCommandFormat::SetupFormat(const ESmithFormatType* srcData, size_t srcSize, uint64 row, uint64 column)
    {
      check(srcData != nullptr)
      check(srcSize == sizeof(ESmithFormatType) * row * column)

      m_formatArr = TDimension2Array<ESmithFormatType>(srcData, srcSize, row, column);
      setCNCoord();
    }

    void FSmithCommandFormat::setCNCoord()
    {
      for (uint64 y = 0; y < GetRow(); ++y)
      {
        for (uint64 x = 0; x < GetColumn(); ++x)
        {
          auto formatType = m_formatArr[y][x];
          if ((formatType == ESmithFormatType::CENTER_NO_EFFECT) || (formatType == ESmithFormatType::CENTER_EFFECT))
          {
            m_cnCoord.x = StaticCast<uint8>(x);
            m_cnCoord.y = StaticCast<uint8>(y);

            return;
          }
        }
      }
    }
  }
}
