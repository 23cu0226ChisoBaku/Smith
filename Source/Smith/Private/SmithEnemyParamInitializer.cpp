// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnemyParamInitializer.h"

IParamInitializer* FSmithEnemyParamInitializer::ParamInitializer = nullptr;

void FSmithEnemyParamInitializer::AssignInitializer(IParamInitializer* initializer)
{
  ParamInitializer = initializer;
}

void FSmithEnemyParamInitializer::DetachInitializer()
{
  ParamInitializer = nullptr;
}