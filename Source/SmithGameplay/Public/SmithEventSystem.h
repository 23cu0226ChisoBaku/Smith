// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IEventExecutor.h"
#include "IEventRegister.h"
#include "UObject/NoExportTypes.h"

#include "UObject/WeakInterfacePtr.h"

#include "SmithEventSystem.generated.h"

UCLASS()
class SMITHGAMEPLAY_API USmithEventSystem : public UObject, public IEventRegister, public IEventExecutor
{
	GENERATED_BODY()

	struct SmithEventHandle
	{
		SmithEventHandle(AActor*, ISmithMapEvent*);
		TWeakObjectPtr<AActor> EventInstigator;
		TWeakInterfacePtr<ISmithMapEvent> Event;
	};

public:
	USmithEventSystem(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:

	void RegisterMapEvent(AActor* Instigator, ISmithMapEvent*) override final;
	void ExecuteEvent() override final;
	bool IsEventInStock() const override final;
	void Reset();

private:

	TArray<SmithEventHandle> m_eventHandleContainer;
};
