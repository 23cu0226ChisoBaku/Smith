// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SMITH_API IOnOffSwitchable
{
public:
	virtual void SwitchOn() = 0;
	virtual void SwitchOff() = 0;
	
	virtual ~IOnOffSwitchable() = 0;
};
