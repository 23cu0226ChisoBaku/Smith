// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithCommandFormat.h"

#include "FormatType.h"

using namespace UE::MLibrary::MDataStructure;

namespace UE::Smith
{
  namespace Battle
  {

    FSmithCommandFormat::FSmithCommandFormat(const ESmithFormatType* srcData, size_t dataCnt, uint64 row, uint64 column)
      : m_formatArr(TDimension2Array<ESmithFormatType>(srcData, dataCnt, row, column))
      , m_cnCoord({})
    { 
      setCNCoord();
    }

    void FSmithCommandFormat::SetupFormat(const ESmithFormatType* srcData, size_t dataCnt, uint64 row, uint64 column)
    {
      check(srcData != nullptr)
      check((dataCnt == (row * column)))

      m_formatArr = TDimension2Array<ESmithFormatType>(srcData, dataCnt, row, column);
      setCNCoord();
    }

    TDimension2Array<ESmithFormatType> FSmithCommandFormat::GetFormatArray() const
    {
      return m_formatArr;
    } 

    FMapCoord FSmithCommandFormat::GetCNCoord() const
    {
      return m_cnCoord;
    }

    uint64 FSmithCommandFormat::GetRow() const
    {
      return m_formatArr.Row();
    }

    uint64 FSmithCommandFormat::GetColumn() const
    {
      return m_formatArr.Column();
    }

    ESmithFormatType FSmithCommandFormat::GetFormatData(uint64 x, uint64 y) const
    {
      return m_formatArr.At_ReadOnly(y, x);
    }
    
    bool FSmithCommandFormat::IsValid() const
    {
      return (GetRow() != 0) && (GetColumn() != 0);
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
  }
}
