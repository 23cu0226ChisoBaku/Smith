// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISmithMapEvent.generated.h"

class ICanSetOnMap;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USmithMapEvent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
// TODO インターフェースを抽象クラスにする
class SMITH_API ISmithMapEvent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitializeEvent(const FVector&, const FRotator& = FRotator::ZeroRotator) = 0;
	virtual void TriggerEvent(ICanSetOnMap*) = 0;
	virtual void DiscardEvent() = 0;
	virtual void RaiseEvent() = 0;
	virtual bool IsDisposed() const = 0;
	void SetEventCoord(uint8 x, uint8 y);
	uint8 GetEventCoordX() const;
	uint8 GetEventCoordY() const;

private:
	uint8 m_eventCoordX = 0u;
	uint8 m_eventCoordY = 0u;
};
