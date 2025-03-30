// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmithMoveDirector.h"

#include "UObject/WeakInterfacePtr.h"
#include "SmithMapBaseMoveDirector.generated.h"

class ITargetTracker;
/**
 * 
 */
UCLASS()
class SMITH_API USmithMapBaseMoveDirector : public USmithMoveDirector
{
	GENERATED_BODY()

public:
	USmithMapBaseMoveDirector(const FObjectInitializer&);

public:
	void Initialize(ITargetTracker*, AActor*, uint8 chaseRadius);

private:
	EDirection getNextDirectionImpl() override;
	
private:
	TWeakInterfacePtr<ITargetTracker> m_targetTracker;
	TWeakObjectPtr<AActor> m_chaser;
	uint8 m_chaseRadius;
};
