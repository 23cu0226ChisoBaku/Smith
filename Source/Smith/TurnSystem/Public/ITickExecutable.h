// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SMITH_API ITickExecutable
{
public:
	virtual void Execute(float DeltaTime) = 0;

	virtual ~ITickExecutable() = 0 {};
};
