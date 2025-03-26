// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnemyParamInitializer.h"

void USmithEnemyParamInitializer::AssignInitializer(TScriptInterface<IParamInitializer> initializer)
{
  gParamInitializer = initializer.GetInterface();
}

void USmithEnemyParamInitializer::DetachInitializer()
{
  gParamInitializer = nullptr;
}

bool USmithEnemyParamInitializer::IsParamInitializerValid()
{
  return IS_UINTERFACE_VALID(gParamInitializer);
}