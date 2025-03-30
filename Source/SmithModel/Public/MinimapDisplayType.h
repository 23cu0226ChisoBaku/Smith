// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EMinimapDisplayType : uint8
{
	Default UMETA(Hidden),
	Ground,
	Wall,
	Corridor,
	Player,
	Enemy,
	Pickable,
	NextLevel,
	DisplayTypeCnt UMETA(Hidden),
};
