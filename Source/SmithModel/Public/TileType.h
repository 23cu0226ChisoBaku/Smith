// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
enum class ETileType : uint8
{
  Void = 0,
	Ground = 1,
  Wall = 2,
  Corridor = 3,
  TileTypeCount UMETA(Hidden),
};
