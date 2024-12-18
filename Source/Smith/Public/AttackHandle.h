// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithCommandFormat.h"

/**
 * 
 */
struct SMITH_API AttackHandle
{
	static const AttackHandle NullHandle;

	FString AttackName;
	int32 AttackPower;
};

const AttackHandle AttackHandle::NullHandle{};
