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

constexpr FMapCoord::FMapCoord()
  : x(0)
  , y(0)
{ }

FMapCoord::FMapCoord(uint8 _x, uint8 _y)
  : x(_x)
  , y(_y)
{ }

FMapCoord::FMapCoord(const FMapCoord& other)
  : x(other.x)
  , y(other.y)
{ }

FMapCoord& FMapCoord::operator=(const FMapCoord& other)
{
  if (this != &other)
  {
    x = other.x;
    y = other.y;
  }

  return *this;
}

bool operator==(const FMapCoord& lhs, const FMapCoord& rhs)
{
  return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FMapCoord& Thing)
{
  uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FMapCoord));
  return Hash;
}
#endif