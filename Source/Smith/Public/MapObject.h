// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//USTRUCT(BlueprintType)
struct  MapCoord
{
	//UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 x;
	//UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 y;
};

/**
 *💩 
 */
class SMITH_API MapObject
{
public:
	MapObject();
	virtual ~MapObject();

	AActor* GetActor()const;

	MapCoord GetCoord()const;
	void SetCoord(MapCoord setterCoord);

private:
	MapCoord myCoord = { 0,0 };
};
