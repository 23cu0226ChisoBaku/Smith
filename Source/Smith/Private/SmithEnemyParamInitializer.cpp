// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnemyParamInitializer.h"

void USmithEnemyParamInitializer::AssignInitializer(TScriptInterface<IParamInitializer> initializer)
{
  gParamInitializer = initializer.GetInterface();
  MDebug::LogError(FString::FromInt(reinterpret_cast<int64>(gParamInitializer)));
  MDebug::LogError(FString::FromInt(reinterpret_cast<int64>(&gParamInitializer)));
  UE_LOG(LogTemp, Log, TEXT("Param %s"), *FString::FromInt(reinterpret_cast<int64>(gParamInitializer)));
  UE_LOG(LogTemp, Log, TEXT("Param Ptr Address %s"), *FString::FromInt(reinterpret_cast<int64>(&gParamInitializer)));
}

void USmithEnemyParamInitializer::DetachInitializer()
{
  gParamInitializer = nullptr;
}

bool USmithEnemyParamInitializer::IsParamInitializerValid()
{
  return IS_UINTERFACE_VALID(gParamInitializer);
}