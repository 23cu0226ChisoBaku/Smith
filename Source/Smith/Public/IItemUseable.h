// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IItemUseable.generated.h"

class USmithHPItem;

UINTERFACE(MinimalAPI)
class UItemUseable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 	アイテム使用ビジター
 */
class SMITH_API IItemUseable
{
	GENERATED_BODY()

public:
	virtual void UseItem(USmithHPItem*) = 0;

};
