// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakInterfacePtr.h"
#include "ISmithMapEvent.h"
#include "SmithPickUpItemEvent.generated.h"

class IPickable;

/**
 * 
 */
UCLASS()
class SMITH_API USmithPickUpItemEvent : public UObject, public ISmithMapEvent
{
	GENERATED_BODY()

public:
	USmithPickUpItemEvent(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	virtual void InitializeEvent(const FVector&) override;
	virtual bool TriggerEvent(ICanSetOnMap*) override;
	virtual void DiscardEvent() override;

public:
	void AssignPickable(IPickable*, AActor*);
	
private:
	UPROPERTY()
	TObjectPtr<AActor> m_pickableAppearence;
	UPROPERTY()
	TObjectPtr<UObject> m_pickableObject;
	TWeakInterfacePtr<IPickable> m_pickable;
};
