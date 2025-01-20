// Fill out your copyright notice in the Description page of Project Settings.


#include "MapObject.h"

MapObject::MapObject()
{

}

MapObject::~MapObject()
{

}

AActor* MapObject::GetActor()const
{
	return nullptr;
}

MapCoord MapObject::GetCoord()const
{
	return myCoord;
}

void MapObject::SetCoord( MapCoord const setCoord)
{
	myCoord = setCoord;
}
