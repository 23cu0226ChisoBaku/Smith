// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_MODEL_HELPER_FUNCTION_LIB
#define SMITH_MODEL_HELPER_FUNCTION_LIB

#include "CoreMinimal.h"

enum class EDirection : uint8;
struct FMapCoord;

/**
 * 
 */
class SMITHMODEL_API FSmithModelHelperFunctionLibrary
{
	private:
		FSmithModelHelperFunctionLibrary() = delete;
		~FSmithModelHelperFunctionLibrary() = delete;

	public:
		static EDirection GetDirectionOfMapCoord(const FMapCoord& from, const FMapCoord& to);
};

#endif