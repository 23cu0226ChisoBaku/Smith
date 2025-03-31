// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakInterfacePtr.h"
#include "ISmithMapEvent.h"
#include "ISmithEventLogger.h"
#include "IMinimapDisplayable.h"
#include "SmithPickUpItemEvent.generated.h"

class IPickable;
class UNiagaraSystem;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class SMITH_API USmithPickUpItemEvent : public UObject, public ISmithMapEvent, 
																				public ISmithEventLogger, public IMinimapDisplayable
{
	GENERATED_BODY()

public:
	USmithPickUpItemEvent(const FObjectInitializer&);
	virtual void BeginDestroy() override;

public:
	virtual void InitializeEvent(const FVector&, const FRotator&) override;
	virtual void TriggerEvent(AActor* Instigator) override;
	virtual void DiscardEvent() override;
	virtual void RaiseEvent() override;
	virtual bool IsDisposed() const override;

	UObject* GetEventEntity() const override;

public:
	void AssignPickable(IPickable*, UNiagaraSystem*);
	IPickable* GetPickable() const;
	FString GetPickUpItemType() const;

public:
UTexture2D* GetMinimapDisplayTexture_Implementation() override final;
	
private:
	UPROPERTY()
	TObjectPtr<UNiagaraSystem> m_itemEventNiagaraSystem;
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> m_itemEventNiagaraComp;
	UPROPERTY()
	TObjectPtr<UObject> m_pickableObject;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> MinimapTexture;

private:
	TWeakInterfacePtr<IPickable> m_pickable;
	uint8 m_isPicked : 1;
};
