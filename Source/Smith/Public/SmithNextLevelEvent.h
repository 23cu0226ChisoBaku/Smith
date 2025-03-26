// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ISmithMapEvent.h"
#include "IMinimapDisplayable.h"
#include "SmithNextLevelEvent.generated.h"

DECLARE_DELEGATE(FOnTriggerEvent);

class ICanSetOnMap;

/**
 * 
 */
UCLASS()
class SMITH_API USmithNextLevelEvent : public UObject, public ISmithMapEvent, public IMinimapDisplayable
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
	UTexture2D* GetMinimapDisplayTexture_Implementation() override final;

public:
	FOnTriggerEvent OnNextLevel;

private:
	UPROPERTY()
	TObjectPtr<AActor> m_eventAppearance;

	// TODO 統一感のある設計にする
	UPROPERTY()
	TObjectPtr<UTexture2D> m_minimapTexture;
	
	uint8 m_bIsDisposed : 1;

};
