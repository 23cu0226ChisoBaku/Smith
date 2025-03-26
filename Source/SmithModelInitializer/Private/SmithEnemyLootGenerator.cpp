// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnemyLootGenerator.h"

void USmithEnemyLootGenerator::AssignLootGenerator(TScriptInterface<ILootGeneratable> generatable)
{
  gLootGenerator = generatable.GetInterface();
}

void USmithEnemyLootGenerator::DetachLootGenerator()
{
  gLootGenerator = nullptr;
}

bool USmithEnemyLootGenerator::IsLootGeneratorValid()
{
  return IS_UINTERFACE_VALID(gLootGenerator);
}