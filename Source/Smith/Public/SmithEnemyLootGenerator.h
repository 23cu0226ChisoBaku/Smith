// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_ENEMY_LOOT_GENERATOR
#define SMITH_ENEMY_LOOT_GENERATOR

#include "CoreMinimal.h"
#include "ILootGeneratable.h"
#include "IPickable.h"
#include <type_traits>
#include "SmithEnemyTraits.h"
#include "MLibrary.h"
/**
 * 
 */
class SMITH_API FSmithEnemyLootGenerator
{
private:
	FSmithEnemyLootGenerator() = delete;
	~FSmithEnemyLootGenerator() = delete;

public:
	static void AssignLootGenerator(ILootGeneratable*);
	static void DetachLootGenerator();

	template<typename EnemyType>
	static IPickable* GetLoot(const EnemyType& Enemy)
	{
		return IS_UINTERFACE_VALID(gLootGenerator) ? gLootGenerator->GetLoot(SmithEnemyTraits<EnemyType, std::is_pointer_v<EnemyType>>::Type::NAME) : nullptr;
	}

private:
	static ILootGeneratable* gLootGenerator;
};

#endif
