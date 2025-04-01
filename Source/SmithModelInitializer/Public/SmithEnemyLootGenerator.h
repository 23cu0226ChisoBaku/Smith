// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include <type_traits>
#include "ILootGeneratable.h"
#include "IPickable.h"
#include "MLibrary.h"
#include "SmithEnemyTraits.h"
#include "UObject/ScriptInterface.h"

#include "SmithEnemyLootGenerator.generated.h"

UCLASS()
class SMITHMODELINITIALIZER_API USmithEnemyLootGenerator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Loot Generator")
	static void AssignLootGenerator(TScriptInterface<ILootGeneratable> generator);

	UFUNCTION(BlueprintCallable, Category = "Loot Generator")
	static void DetachLootGenerator();

	UFUNCTION(BlueprintCallable, Category = "Loot Generator")
	static bool IsLootGeneratorValid();

	template<typename EnemyType>
	static IPickable* GetLoot(const EnemyType& Enemy)
	{
		return IS_UINTERFACE_VALID(gLootGenerator) ? gLootGenerator->GetLoot(SmithEnemyTraits<EnemyType, std::is_pointer_v<EnemyType>>::Type::NAME) : nullptr;
	}

private:

	static inline ILootGeneratable* gLootGenerator = nullptr;
};