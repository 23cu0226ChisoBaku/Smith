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
          mapCoords.At(y, x).x = cnMapCoord.x - cnCoord.x + StaticCast<uint8>(x);
          mapCoords.At(y, x).y = cnMapCoord.y - cnCoord.y + StaticCast<uint8>(y);
        }
      }

      return mapCoords;
    }
  }
}
