// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include <type_traits>
#include "IParamInitializer.h"
#include "MLibrary.h"
#include "UObject/ScriptInterface.h"

#include "SmithEnemyParamInitializer.generated.h"

UCLASS()
class SMITHMODELINITIALIZER_API USmithEnemyParamInitializer : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Parameter Initializer")
	static void AssignInitializer(TScriptInterface<IParamInitializer> initializer);

	UFUNCTION(BlueprintCallable, Category = "Parameter Initializer")
	static void DetachInitializer();
	
	UFUNCTION(BlueprintCallable, Category = "Parameter Initializer")
	static bool IsParamInitializerValid();
	
	// 敵パラメーター初期化関数
	template<typename EnemyType>
	static FParams GetParams(const EnemyType& Enemy, int32 currentLevel)
	{
		return IS_UINTERFACE_VALID(gParamInitializer) ? gParamInitializer->Initialize(typename SmithEnemyTraits<EnemyType, std::is_pointer_v<EnemyType>>::Type{}, currentLevel) : FParams{};
	}

private:
	static inline IParamInitializer* gParamInitializer = nullptr;
};
