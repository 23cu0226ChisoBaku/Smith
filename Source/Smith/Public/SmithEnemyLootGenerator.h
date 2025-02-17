// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ScriptInterface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ILootGeneratable.h"
#include "IPickable.h"
#include <type_traits>
#include "SmithEnemyTraits.h"
#include "MLibrary.h"
#include "SmithEnemyLootGenerator.generated.h"

UCLASS()
class SMITH_API USmithEnemyLootGenerator : public UBlueprintFunctionLibrary
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
	static IPickable* GetLoot(const EnemyType& Enemy);

private:
	static ILootGeneratable* gLootGenerator;
};

template<typename EnemyType>
static IPickable* USmithEnemyLootGenerator::GetLoot(const EnemyType& Enemy)
{
	return IS_UINTERFACE_VALID(gLootGenerator) ? gLootGenerator->GetLoot(SmithEnemyTraits<EnemyType, std::is_pointer_v<EnemyType>>::Type::NAME) : nullptr;
}

