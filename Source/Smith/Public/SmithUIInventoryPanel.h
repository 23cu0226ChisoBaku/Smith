// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithUIInventoryPanel.generated.h"

class UListView;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithUIInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	USmithUIInventoryPanel(const FObjectInitializer&);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> InventoryListView;
	
};
