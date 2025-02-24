// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SmithMinimapTileEntry.generated.h"

class UTexture2D;
class UImage;

UCLASS(Abstract)
class SMITH_API USmithMinimapTileEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeOnListItemObjectSet(UObject* listItemObject) override;

public:
	void SetTileTexture(UTexture2D*);
	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> TileImage;
};
