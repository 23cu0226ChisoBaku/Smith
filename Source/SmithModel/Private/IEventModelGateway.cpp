// Fill out your copyright notice in the Description page of Project Settings.

#include "IEventModelGateway.h"

bool operator==(const FEventModelDTO& Lhs, const FEventModelDTO& Rhs)
{
  return Lhs.EventClass == Rhs.EventClass;
}

