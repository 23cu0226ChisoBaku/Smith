// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IMinimapDisplayer.h"
#include "MapObjType.h"
#include "SmithMinimap.generated.h"

class UImage;
class USmithMinimapTileView;

UCLASS(Abstract)
class SMITH_API USmithMinimap : public UUserWidget, public IMinimapDisplayer
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
	
public:
	virtual void AssignMapData(const TArray<UObject*>& mapItems, uint8 row, uint8 column) override;
	virtual void ReplaceTiles(const TArray<UObject*>&, const FMapCoord& originCoord, uint8 sizeX, uint8 sizeY) override;
	virtual void UpdateMinimap(UObject*, const FMapCoord&, uint8 sizeX, uint8 sizeY) override;
	virtual void SetVisibility(bool bIsVisible) override;
	virtual void ResetMap() override;

private:
	void adjustMinimapPosition();

private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> MinimapBackground;
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<USmithMinimapTileView> Minimap;

	UPROPERTY(EditAnywhere, Category = "Minimap Tile Widget", meta = (MustImplement = "UserObjectListEntry"))
	TSubclassOf<UUserWidget> MinimapTileEntrySubclass;

	UPROPERTY(EditAnywhere, Category = "Minimap Tile Widget")
	TMap<EMapObjType, TObjectPtr<UTexture2D>> TileTables;
};
