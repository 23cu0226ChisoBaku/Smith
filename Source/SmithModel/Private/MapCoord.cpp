// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Map Coordinate

Author : MAI ZHICONG

Description : Map Coordinate(Use by Map)

Update History: 2024/11/28 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "MapCoord.h"

#include "Misc/Crc.h"

FMapCoord::FMapCoord(uint8 _x, uint8 _y)
  : x(_x)
  , y(_y)
{ }

FString FMapCoord::ToString() const
{
  FString str{};

  str.Append(TEXT("X = "));
  str.Append(FString::FromInt(x));
  str.Append(TEXT(", Y = "));
  str.Append(FString::FromInt(y));

  return str;
}

bool operator==(const FMapCoord& lhs, const FMapCoord& rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

FMapCoord operator+(const FMapCoord& lhs, const FMapCoord& rhs)
{
  return FMapCoord(lhs.x + rhs.x, lhs.y + rhs.y);
}

FMapCoord operator-(const FMapCoord& lhs, const FMapCoord& rhs)
{
  return FMapCoord(lhs.x - rhs.x, lhs.y - rhs.y);
}


#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FMapCoord& Thing)
{
  uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FMapCoord));
  return Hash;
}
#endif