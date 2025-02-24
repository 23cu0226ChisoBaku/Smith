// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IMinimapDecorator.h"
#include "TileType.h"
#include "MapObjType.h"
#include "SmithMinimapDecorator.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SMITHGOD_API USmithMinimapDecorator : public UObject, public IMinimapDecorator
{
	GENERATED_BODY()

protected:
	enum : uint8
	{
		// TileType
		Void = ETileType::Void,
		Ground = ETileType::Ground,
		Wall = ETileType::Wall,
		Corridor = ETileType::Corridor,
		// MapObjType
		Player = (uint8)ETileType::TileTypeCount + (uint8)EMapObjType::Player,
		Enemy = (uint8)ETileType::TileTypeCount + (uint8)EMapObjType::Enemy,
		Pickable = (uint8)ETileType::TileTypeCount + (uint8)EMapObjType::Pickable,
		NextLevel = (uint8)ETileType::TileTypeCount + (uint8)EMapObjType::NextLevel,

		Invalid = 255u
	};


public:
	static uint8 ConvertToTileType(EMapObjType);
	static uint8 ConvertToTileType(ETileType);
	
public:
	void AssignDecorator(IMinimapDecorator*);
	virtual void UpdateTile(uint8 tileData, uint8 x, uint8 y, UTexture2D*) override;

public:
	virtual void PostInitProperties() override;

private:
	virtual void BeginPlay();


protected:
	UPROPERTY()
	TScriptInterface<IMinimapDecorator> m_minimapDecorator;
};
