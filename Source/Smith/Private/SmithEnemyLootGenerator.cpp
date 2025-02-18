// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnemyLootGenerator.h"

void USmithEnemyLootGenerator::AssignLootGenerator(TScriptInterface<ILootGeneratable> generatable)
{
  gLootGenerator = generatable.GetInterface();

  MDebug::LogError(FString::FromInt(reinterpret_cast<int64>(gLootGenerator)));
  MDebug::LogError(FString::FromInt(reinterpret_cast<int64>(&gLootGenerator)));
  UE_LOG(LogTemp, Log, TEXT("Loot %s"), *FString::FromInt(reinterpret_cast<int64>(gLootGenerator)));
  UE_LOG(LogTemp, Log, TEXT("Loot Ptr Address %s"), *FString::FromInt(reinterpret_cast<int64>(&gLootGenerator)));
}

void USmithEnemyLootGenerator::DetachLootGenerator()
{
  gLootGenerator = nullptr;
}

bool USmithEnemyLootGenerator::IsLootGeneratorValid()
{
  return IS_UINTERFACE_VALID(gLootGenerator);
}