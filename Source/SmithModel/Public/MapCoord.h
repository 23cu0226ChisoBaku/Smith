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

#include "CoreMinimal.h"
#include "MapCoord.generated.h"

USTRUCT(BlueprintType)
struct SMITHMODEL_API FMapCoord
{
	GENERATED_BODY()

	public:
		constexpr FMapCoord()
		  : x(0)
  		, y(0)
		{ }
		FMapCoord(uint8 _x, uint8 _y);
		FMapCoord(const FMapCoord&);
		FMapCoord& operator=(const FMapCoord&);
		~FMapCoord() = default;
		FString ToString() const;
	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 x;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
