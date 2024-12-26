// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "ITurnManageable.h"
#include "ITurnManageableWrapper.generated.h"

/// @brief ITurnManageableへのポインターの配列のラッパ
/// TMapに入れるため、USTRUCTでラップする必要がある
USTRUCT(BlueprintType)
struct TURNBATTLESYSTEM_API FITurnManageableWrapper
{
	GENERATED_BODY()

	TArray<TWeakInterfacePtr<ITurnManageable>> Elements;

};