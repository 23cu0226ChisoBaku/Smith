// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnemyParamInitializer.h"

IParamInitializer* FSmithEnemyParamInitializer::gParamInitializer = nullptr;

void FSmithEnemyParamInitializer::AssignInitializer(IParamInitializer* initializer)
{
  gParamInitializer = initializer;
}

void FSmithEnemyParamInitializer::DetachInitializer()
{
  gParamInitializer = nullptr;
}