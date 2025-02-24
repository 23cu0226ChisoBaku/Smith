// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmithMinimapDecorator.h"
#include "SmithMapObjectDecorator.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class SMITHGOD_API USmithMapObjectDecorator : public USmithMinimapDecorator
{
	GENERATED_BODY()

public:
	virtual void UpdateTile(uint8 tileData, uint8 x, uint8 y, UTexture2D*) override;

private:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, meta=(RequiredAssetDataTags = "RowStructure=/Script/SmithModel.SmithMapObjDisplayRow"))
	TObjectPtr<UDataTable> MapObjectDisplayRow;
	
	TMap<uint8,TObjectPtr<UTexture2D>> m_mapObjDisplayTable;
	
};
