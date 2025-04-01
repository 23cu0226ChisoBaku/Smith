// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IEventTriggerable.generated.h"

class USmithNextLevelEvent;
class USmithPickUpItemEvent;

UINTERFACE(MinimalAPI)
class UEventTriggerable : public UInterface
{
	GENERATED_BODY()
};

///
/// @brief 			イベントビジター
///
class SMITH_API IEventTriggerable
{
	GENERATED_BODY()

public:
	virtual void OnTriggerEvent(USmithNextLevelEvent*) = 0;
	virtual void OnTriggerEvent(USmithPickUpItemEvent*) = 0;
};
