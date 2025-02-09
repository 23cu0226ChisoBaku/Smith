// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakInterfacePtr.h"
#include "ISmithMapEvent.h"
#include "ISmithEventLogger.h"
#include "SmithPickUpItemEvent.generated.h"

class IPickable;
class UNiagaraSystem;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class SMITH_API USmithPickUpItemEvent : public UObject, public ISmithMapEvent, public ISmithEventLogger
{
	GENERATED_BODY()

public:
	USmithPickUpItemEvent(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	virtual void InitializeEvent(const FVector&, const FRotator&) override;
	virtual void TriggerEvent(ICanSetOnMap*) override;
	virtual void DiscardEvent() override;
	virtual void RaiseEvent() override;
	virtual bool IsDisposed() const override;

	ISmithBattleLogger* GetEventEntityLogger() const override;
	FString GetEventName() const override;
	FString GetSucceedMessage() const override;
	FString GetFailedMessage() const override;

public:
	void AssignPickable(IPickable*, UNiagaraSystem*);
	IPickable* GetPickable() const;
	FString GetPickUpItemType() const;
	
private:
	UPROPERTY()
	TObjectPtr<UNiagaraSystem> m_itemEventNiagaraSystem;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> m_itemEventNiagaraComp;
	UPROPERTY()
	TObjectPtr<UObject> m_pickableObject;

private:
	TWeakInterfacePtr<IPickable> m_pickable;
	uint8 m_isPicked : 1;
};
