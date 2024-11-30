// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithCommandFormat.h"

namespace UE::Smith
{
  inline namespace Battle
  {
    FSmithCommandFormat::FSmithCommandFormat(const int8* srcData, const size_t dataCnt, const uint64 row, const uint64 column)
      : m_formatArr(UE::MLibrary::MDataStructure::TDimension2Array<int8>(srcData, dataCnt, row, column))
    { }

    FSmithCommandFormat::~FSmithCommandFormat()
    { }

    FSmithCommandFormat::FSmithCommandFormat(const FSmithCommandFormat& other)
    {
      this->m_formatArr = other.m_formatArr;
    }

    FSmithCommandFormat& FSmithCommandFormat::operator=(const FSmithCommandFormat& other)
    {
      this->m_formatArr = other.m_formatArr;

      return *this;
    }

    FSmithCommandFormat::FSmithCommandFormat(FSmithCommandFormat&& other) noexcept
    {
      this->m_formatArr = ::MoveTemp(other.m_formatArr);
      other.m_formatArr = UE::MLibrary::MDataStructure::TDimension2Array<int8>::EmptyArray;
    }

    FSmithCommandFormat& FSmithCommandFormat::operator=(FSmithCommandFormat&& other) noexcept
    {
      this->m_formatArr = ::MoveTemp(other.m_formatArr);
      other.m_formatArr = UE::MLibrary::MDataStructure::TDimension2Array<int8>::EmptyArray;

      return *this;
    }

    uint64 FSmithCommandFormat::GetRow() const
    {
      return m_formatArr.Row();
    }

    uint64 FSmithCommandFormat::GetColumn() const
    {
      return m_formatArr.Column();
    }

    UE::MLibrary::MDataStructure::TDimension2Array<int8> FSmithCommandFormat::GetFormatArray() const
    {
      return m_formatArr;
    }

    void FSmithCommandFormat::setCNCoord()
    {
      for (uint64 y = 0; y < GetRow(); ++y)
      {
        for (uint64 x = 0; x < GetColumn(); ++x)
        {
          
        }
      }
    }
  }
}
