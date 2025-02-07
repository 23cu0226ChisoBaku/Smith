// Fill out your copyright notice in the Description page of Project Settings.


#include "ISmithEventLogger.h"
#include "ISmithBattleLogger.h"

// Add default functionality here for any ISmithEventLogger functions that are not pure virtual.

FString ISmithEventLogger::GetEventName() const
{
  return TEXT("とあるイベント");
}

FString ISmithEventLogger::GetSucceedMessage() const
{
  return TEXT("発動成功");
}

FString ISmithEventLogger::GetFailedMessage() const
{
  return TEXT("発動失敗");
}

ISmithBattleLogger* ISmithEventLogger::GetEventEntityLogger() const
{
  return nullptr;
}