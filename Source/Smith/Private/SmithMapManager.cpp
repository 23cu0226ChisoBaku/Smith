
#include "SmithMapManager.h"
#

SmithMapManager::SmithMapManager()
{

}

SmithMapManager::~SmithMapManager()
{

}

MapCoord SmithMapManager::GetObjectCorrd(MapObject* Obj)
{
	return (MapCoord) ( 1, 1 );
}

void SmithMapManager::RegistraitonObject(MapObject* Obj)
{
	Objects.Add(Obj);
}

void SmithMapManager::ReleaseObject(MapObject* Obj)
{
	Objects.Remove(Obj);
}

void SmithMapManager::UpdateObject(MapObject* Obj, MapCoord coord)
{
	MapObject* setterObject =GetObject(Obj);
	setterObject->SetCoord(coord);
}

MapObject* SmithMapManager::GetObject(MapObject* Obj)
{
	for (int i = 0; i < Objects.Num(); ++i)
	{
		if (Objects[i] == Obj)
		{
			return Objects[i];
		}
	}
	return nullptr;
}

MapObject::MapObject(AActor* actor)
{
	p_actor = actor;
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

void MapObject::SetCoord(MapCoord setCoord)
{
	myCoord = setCoord;
}

