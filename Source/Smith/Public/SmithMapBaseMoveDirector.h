// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithMoveDirector.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithMapBaseMoveDirector.generated.h"

class ITargetTracker;
class ICanSetOnMap;
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
	void Initialize(ITargetTracker*, ICanSetOnMap*, uint8 chaseRadius);

private:
	EDirection getNextDirectionImpl() override;
	
private:
	TWeakInterfacePtr<ITargetTracker> m_targetTracker;
	TWeakInterfacePtr<ICanSetOnMap> m_chaser;
	uint8 m_chaseRadius;
};
