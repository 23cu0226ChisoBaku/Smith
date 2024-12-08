// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(Blueprintable)
enum class ETurnPriority : uint8
{
	PlayerSelf = 0,
	Rival = 1,
	Alliance = 2,
};

