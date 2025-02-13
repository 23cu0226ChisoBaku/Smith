// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnemyLootGenerator.h"

void FSmithEnemyLootGenerator::AssignLootGenerator(ILootGeneratable* generatable)
{
  gLootGenerator = generatable;
}

void FSmithEnemyLootGenerator::DetachLootGenerator()
{
  gLootGenerator = nullptr;
}

ILootGeneratable* FSmithEnemyLootGenerator::gLootGenerator = nullptr;