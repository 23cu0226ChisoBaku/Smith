// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakInterfacePtr.h"
#include "IEventRegister.h"
#include "IEventExecutor.h"
#include "SmithEventSystem.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithEventSystem : public UObject, public IEventRegister, public IEventExecutor
{
	GENERATED_BODY()

public:
	USmithEventSystem(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	void RegisterMapEvent(ICanSetOnMap*, ISmithMapEvent*) override final;
	void ExecuteEvent() override final;
	bool IsEventInStock() const override final;
	void Reset();
private:
	struct SmithEventHandle
	{
		SmithEventHandle(ICanSetOnMap*, ISmithMapEvent*);
		TWeakInterfacePtr<ICanSetOnMap> EventReceiver;
		TWeakInterfacePtr<ISmithMapEvent> Event;
	};
	TArray<SmithEventHandle> m_eventHandleContainer;
};
