// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct map
{
	uint8 x;
	uint8 y;
};

class SMITH_API MapObject
{
public:
	MapObject(){}
	virtual ~MapObject()
	{}

	bool Mpve(){return true;}
	int GetAttack() {return 0;}
};


struct tileInfo
{

	MapObject* mapobj;
};

/**
 * 
 */
class SMITH_API SmithMapManager
{
public:
	SmithMapManager();
	virtual ~SmithMapManager();

	
private:
	FVector const originWorldCord;
};
