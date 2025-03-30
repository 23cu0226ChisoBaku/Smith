// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Command Format

Author : MAI ZHICONG

Description : Command Format(Use by ActionCommand)

Update History: 2024/11/25 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_CMD_FORMAT
#define SMITH_CMD_FORMAT

#include "DataStructure/Dimension2Array.h"
#include "MapCoord.h"

enum class ESmithFormatType : uint8;

namespace UE::Smith
{
  namespace Battle
  {
    ///
    /// @brief コマンドフォーマット
    ///
    class SMITHMODEL_API FSmithCommandFormat
    {
      using ThisClass = typename FSmithCommandFormat;
      public:
      
        FSmithCommandFormat(const ESmithFormatType* srcData, size_t dataCnt, uint64 row, uint64 column);

        FSmithCommandFormat() = default;
        ~FSmithCommandFormat() = default;
        FSmithCommandFormat(const ThisClass&) = default;
        ThisClass& operator=(const ThisClass&) = default;
        FSmithCommandFormat(ThisClass&&) noexcept = default;
        ThisClass& operator=(ThisClass&&) noexcept = default;

      public:

        void SetupFormat(const ESmithFormatType* srcData, size_t dataCnt, uint64 row, uint64 column);

        UE::MLibrary::MDataStructure::TDimension2Array<ESmithFormatType> GetFormatArray() const;
        FMapCoord GetCNCoord() const;
        uint64 GetRow() const;
        uint64 GetColumn() const;
        ESmithFormatType GetFormatData(uint64 x, uint64 y) const;
        bool IsValid() const;

      private:

        void setCNCoord();

      private:

        UE::MLibrary::MDataStructure::TDimension2Array<ESmithFormatType> m_formatArr;
        FMapCoord m_cnCoord;
    };
  }
}

#endif