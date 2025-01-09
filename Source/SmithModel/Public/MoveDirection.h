// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace UE::Smith
{
  namespace Battle
  {
    enum class EMoveDirection: uint8
    {
      North = 0,
      NorthEast = 1,
      East = 2,
      SouthEast = 3,
      South = 4,
      SouthWest = 5,
      West = 6,
      NorthWest = 7,

      DirectionCount,
      None = 0xff,
    };
  }
}
