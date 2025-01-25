// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SmithUIInventoryEntry.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithUIInventoryEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	USmithUIInventoryEntry(const FObjectInitializer&);

protected:
	virtual void NativeOnListItemObjectSet(UObject* listItemObject) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIconImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameLabel;
	
};
