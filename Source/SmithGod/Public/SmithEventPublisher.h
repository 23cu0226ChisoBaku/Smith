// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ISmithMapEvent.h"
#include "IPickable.h"
#include "MLibrary.h"
#include "SmithEventPublisher.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithEventPublisher : public UObject
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
			MDebug::Log("Null UClass");
			return nullptr;
		}

		// TODO
		if (!inEventStaticClass->ImplementsInterface(USmithMapEvent::StaticClass()))
		{
			MDebug::Log("Not child ----");
			return nullptr;
		}

		return NewObject<T>(GetWorld());
	}
};
