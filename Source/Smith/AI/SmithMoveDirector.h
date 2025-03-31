// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Direction.h"
#include "SmithMoveDirector.generated.h"

/**
 * 
 */
UCLASS(Abstract,EditInlineNew)
class SMITH_API USmithMoveDirector : public UObject
{
	GENERATED_BODY()

public:
	USmithMoveDirector(const FObjectInitializer&);

public:
	EDirection GetNextDirection();

private:
	virtual EDirection getNextDirectionImpl() PURE_VIRTUAL(USmithMoveDirector::getNextDirectionImpl, return EDirection::Invalid;);
	
};
