// Fill out your copyright notice in the Description page of Project Settings.
/*

FormatTransformer.cpp

Author : MAI ZHICONG

Description : Format Transformer class

Update History: 2024/11/29 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "FormatTransformer.h"

#include "SmithCommandFormat.h"
#include "MapCoord.h"
#include "Direction.h"
#include "FormatType.h"

#include "MLibrary.h"

using namespace UE::MLibrary::MDataStructure;

namespace UE::Smith
{
  namespace Battle
  {
    TDimension2Array<FMapCoord> FFormatTransformer::FormatToMapCoord(
                                                                      const FSmithCommandFormat& format, 
                                                                      FMapCoord cnMapCoord
                                                                    )
    {
      TDimension2Array<FMapCoord> mapCoords(format.GetRow(), format.GetColumn());
      FMapCoord cnCoord = format.GetCNCoord();
      mapCoords.At(cnCoord.y, cnCoord.x) = cnMapCoord;

      for (uint64 y = 0; y < format.GetRow(); ++y)
      {
        for (uint64 x = 0; x < format.GetColumn(); ++x)
        {
          // TODO need check if result x/y out of bounds 
          mapCoords.At(y, x).x = cnMapCoord.x - cnCoord.x + StaticCast<uint8>(x);
          mapCoords.At(y, x).y = cnMapCoord.y - cnCoord.y + StaticCast<uint8>(y);
        }
      }

      return mapCoords;
    }

    FSmithCommandFormat FFormatTransformer::GetRotatedFormat(const FSmithCommandFormat& src, EDirection direction)
    {
      if (StaticCast<uint8>(direction) >= StaticCast<uint8>(EDirection::DirectionCount))
      {
        return src;
      }

      // 上下左右に調整する(斜め回転できない)
      const EDirection direction_adjust = StaticCast<EDirection>(StaticCast<uint8>(direction) - (StaticCast<uint8>(direction) % 2));

      if (direction_adjust == EDirection::North)
      {
        return src;
      }
      else
      {
        return getRotatedFormatImpl(src, direction_adjust);
      }
    }

    FSmithCommandFormat FFormatTransformer::getRotatedFormatImpl(const FSmithCommandFormat& src, EDirection direction)
    {
      const uint64 srcFormatRow = src.GetRow();
      const uint64 srcFormatColumn = src.GetColumn();
      check((srcFormatRow > 0) && (srcFormatColumn > 0));

      uint64 rotatedFormatRow = 0;
      uint64 rotatedFormatColumn = 0;
      FSmithCommandFormat rotatedFormat{};
      TArray<ESmithFormatType> srcData{};

      using enum EDirection;
      switch (direction)
      {
        case East:
        {
          for (int x = 0; x < srcFormatColumn; ++x)
          {
            for (int y = srcFormatRow - 1; y >= 0; --y)
            {
              srcData.Emplace(src.GetFormatData(x, y));
            }
          }
          rotatedFormatRow = srcFormatColumn;
          rotatedFormatColumn = srcFormatRow;
        }
        break;
        case South:
        {
          for (int y = srcFormatRow - 1; y >= 0; --y)
          {
            for (int x = srcFormatColumn - 1; x >= 0; --x)
            {
              srcData.Emplace(src.GetFormatData(x, y));
            }
          }
          rotatedFormatRow = srcFormatRow;
          rotatedFormatColumn = srcFormatColumn;
        }
        break;
        case West:
        {
          for (int x = srcFormatColumn - 1; x >=0; --x)
          {
            for (int y = 0; y < srcFormatRow; ++y)
            {
              srcData.Emplace(src.GetFormatData(x, y));
            }
          }
          rotatedFormatRow = srcFormatColumn;
          rotatedFormatColumn = srcFormatRow;
        }
        break;
        default:
        {
          MDebug::LogError("Format Rotation Failed ----- invalid direcion");
          break;
        }
      }

      if (srcData.Num() != 0)
      {
        const size_t dataCount = StaticCast<size_t>(srcData.Num()); 
        rotatedFormat.SetupFormat(srcData.GetData(), dataCount, rotatedFormatRow, rotatedFormatColumn);
      }

      return rotatedFormat;
    }
  }
}
