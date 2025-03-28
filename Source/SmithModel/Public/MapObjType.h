// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EMapObjType : uint8
{
  Player UMETA(DisplayName = "Player"),
  Enemy UMETA(DisplayName = "Enemy"),
  Pickable UMETA(DisplayName = "Pickable Item"),
  NextLevel UMETA(DisplayName = "Next Level"),
  TypeCount UMETA(Hidden),
};	
