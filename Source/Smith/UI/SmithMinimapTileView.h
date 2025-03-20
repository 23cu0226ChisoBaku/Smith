// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SmithMinimapTileView.generated.h"

class UTileView;
class USmithMinimapTileEntry;
/**
 * 
 */
UCLASS(Abstract)
class SMITH_API USmithMinimapTileView : public UUserWidget
{
	GENERATED_BODY()

public:
	USmithMinimapTileView(const FObjectInitializer&);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void InitMinimap(const TArray<UObject*>& mapTiles, uint8 row, uint8 column);
	void ReplaceTile(UObject*, uint8 x, uint8 y);
	void SetTile(UObject*, uint8 x, uint8 y);
	void SetTileVisible(uint8 x, uint8 y);
	void EmptyTiles();

private:
	void setTileImpl(UObject*, uint8 x, uint8 y);
	void replaceTileItemImpl(UObject*, uint8 x, uint8 y);
	USmithMinimapTileEntry* getTileEntryInternal(uint8 x, uint8 y) const;
	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTileView> MinimapTiles;

private:
	uint8 m_minimapRow;
	uint8 m_minimapColumn;
};
