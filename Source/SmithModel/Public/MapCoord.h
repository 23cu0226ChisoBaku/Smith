// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Map Coordinate

Author : MAI ZHICONG

Description : Map Coordinate(Use by Map)

Update History: 2024/11/28 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

struct SMITHMODEL_API FMapCoord
{

  public:

    constexpr FMapCoord()
      : x(0)
      , y(0)
    { }

    FMapCoord(uint8 x_, uint8 y_);
    ~FMapCoord() = default;

    FMapCoord(const FMapCoord& Other) = default;
    FMapCoord& operator=(const FMapCoord& Other) = default;

    FMapCoord(FMapCoord&& Other) noexcept = default;
    FMapCoord& operator=(FMapCoord&& Other) noexcept = default;

    FString ToString() const;
    
  public:
  
    uint8 x;

    uint8 y;
};

/// @brief 比較演算子
SMITHMODEL_API bool operator==(const FMapCoord&, const FMapCoord&);
SMITHMODEL_API FMapCoord operator+(const FMapCoord&, const FMapCoord&);
SMITHMODEL_API FMapCoord operator-(const FMapCoord&, const FMapCoord&);

#if UE_BUILD_DEBUG
uint32 GetTypeHash(const FMapCoord&);
#else /// @brief optimize by inlining in shipping and development builds
__forceinline uint32 GetTypeHash(const FMapCoord& Thing)
{
  uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FMapCoord));
  return Hash;
}
#endif
