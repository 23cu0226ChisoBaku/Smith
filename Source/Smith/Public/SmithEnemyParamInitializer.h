// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_ENEMY_PARAM_INITIALIZER
#define SMITH_ENEMY_PARAM_INITIALIZER

#include "CoreMinimal.h"
#include "IParamInitializer.h"
#include "MLibrary.h"

/**
 * 
 */
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
	static FParams GetParams(EnemyType* InstancePtr, int32 currentLevel)
	{
		return IS_UINTERFACE_VALID(ParamInitializer) ? ParamInitializer->Initialize(typename SmithEnemyTraits<EnemyType>::Type{}, currentLevel) : FParams{};
	}

private:
	static IParamInitializer* ParamInitializer;
};

#endif