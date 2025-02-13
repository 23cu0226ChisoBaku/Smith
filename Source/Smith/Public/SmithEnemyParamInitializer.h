// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_ENEMY_PARAM_INITIALIZER
#define SMITH_ENEMY_PARAM_INITIALIZER

#include "CoreMinimal.h"
#include "IParamInitializer.h"
#include <type_traits>
#include "MLibrary.h"

class SMITH_API FSmithEnemyParamInitializer
{
private:
	FSmithEnemyParamInitializer() = delete;
	~FSmithEnemyParamInitializer() = delete;

public:
	static void AssignInitializer(IParamInitializer*);
	static void DetachInitializer();
	// 敵パラメーター初期化関数
	template<typename EnemyType>
	static FParams GetParams(const EnemyType& Enemy, int32 currentLevel)
	{
		return IS_UINTERFACE_VALID(gParamInitializer) ? gParamInitializer->Initialize(typename SmithEnemyTraits<EnemyType, std::is_pointer_v<EnemyType>>::Type{}, currentLevel) : FParams{};
	}

private:
	static IParamInitializer* gParamInitializer;
};

#endif