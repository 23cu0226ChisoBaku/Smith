// Fill out your copyright notice in the Description page of Project Settings.


#include "ISmithBattleLogger.h"

// Add default functionality here for any ISmithBattleLogger functions that are not pure virtual.

FString ISmithBattleLogger::GetName_Log() const
{
  return TEXT("無名");
}

EBattleLogType ISmithBattleLogger::GetType_Log() const
{
  return EBattleLogType::None;
}