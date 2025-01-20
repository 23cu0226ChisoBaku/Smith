// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ISmithMapEvent.h"
#include "SmithNextLevelEvent.generated.h"

DECLARE_DELEGATE(FOnTriggerEvent);

class ICanSetOnMap;

/**
 * 
 */
UCLASS()
class SMITH_API USmithNextLevelEvent : public UObject, public ISmithMapEvent
{
	GENERATED_BODY()

public:
	USmithNextLevelEvent(const FObjectInitializer&);
	virtual void BeginDestroy() override;
	
public:
	void InitializeEvent(const FVector&) override final;
	bool TriggerEvent(ICanSetOnMap*) override final;
	void DiscardEvent() override final;

public:
	FOnTriggerEvent OnNextLevel;

private:
	UPROPERTY()
	TObjectPtr<AActor> m_eventAppearance;

};
