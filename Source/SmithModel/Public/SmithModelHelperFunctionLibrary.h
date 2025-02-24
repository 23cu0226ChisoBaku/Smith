// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MODEL_HELPER_FUNCTION_LIB
#define SMITH_MODEL_HELPER_FUNCTION_LIB

#include "CoreMinimal.h"

enum class EDirection : uint8;
struct FMapCoord;

enum class EDirectionStrategy : uint8
{
	// 八方位
	Ordinal,
	// 四方位
	Cardinal,
	// 四方位(斜め)
	Diagonal,
};

class SMITHMODEL_API FSmithModelHelperFunctionLibrary
{
	private:
		FSmithModelHelperFunctionLibrary() = delete;
		~FSmithModelHelperFunctionLibrary() = delete;

	public:
		static EDirection GetDirectionOfMapCoord(const FMapCoord& from, const FMapCoord& to, EDirectionStrategy = EDirectionStrategy::Ordinal);
		static EDirection GetDirectionOfDegree(double angleDegree, EDirectionStrategy = EDirectionStrategy::Ordinal);
	private:
		static void adjustInputAngle(double& angleDegree);
};

#endif