// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"

#include "ISmithMapEvent.h"

#include "SmithEventPublisher.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGAMEPLAY_API USmithEventPublisher : public UObject
{
	GENERATED_BODY()

public:
	USmithEventPublisher(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	template<typename T>
	T* PublishMapEvent(UClass* inEventStaticClass)
	{
		if (inEventStaticClass == nullptr)
		{
			return nullptr;
		}

		// TODO
		if (!inEventStaticClass->ImplementsInterface(USmithMapEvent::StaticClass()))
		{
			return nullptr;
		}

		return NewObject<T>(GetWorld());
	}
};
