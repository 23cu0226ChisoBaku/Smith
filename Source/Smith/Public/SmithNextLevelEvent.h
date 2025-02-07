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
	void InitializeEvent(const FVector&, const FRotator&) override final;
	void TriggerEvent(ICanSetOnMap*) override final;
	void DiscardEvent() override final;
	void RaiseEvent() override final;
	bool IsDisposed() const override final;

public:
	FOnTriggerEvent OnNextLevel;

private:
	UPROPERTY()
	TObjectPtr<AActor> m_eventAppearance;
	uint8 m_bIsDisposed : 1;

};
