// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(Blueprintable)
enum class ETileType : uint8
{
  Void = 0,
	Ground = 1,
  Wall = 2,
  Corridor = 3,
  
};