// Fill out your copyright notice in the Description page of Project Settings.


#include "ISmithEventLogger.h"

// Add default functionality here for any ISmithEventLogger functions that are not pure virtual.

UObject* ISmithEventLogger::GetEventEntity() const
{
  return _getUObject();
}