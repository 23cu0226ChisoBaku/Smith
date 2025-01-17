// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMoveDirector.h"

USmithMoveDirector::USmithMoveDirector(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
{ }

EDirection USmithMoveDirector::GetNextDirection()
{
  return getNextDirectionImpl();
}