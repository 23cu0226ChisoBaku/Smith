
#include "SmithMapManager.h"
#

SmithMapManager::SmithMapManager()
{

}

SmithMapManager::~SmithMapManager()
{

}

MapCoord SmithMapManager::GetObjectCorrd(MapObject* searchObj)
{
	MapObject* currentMapObj;
	for (int i = 0; i << Actors.Num(); ++i)
	{
		currentMapObj = Actors[i];
	}
	return currentMapObj->GetCoord();
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

