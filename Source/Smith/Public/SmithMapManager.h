// MapManager
// ã–ìŒ’“l


#pragma once

#include "CoreMinimal.h"

// ‰¼’u‚« UŒ‚”ÍˆÍ‚Æ‚©‚Ìî•ñ‚ğ‚ÂH
struct Format
{

};

//USTRUCT(BlueprintType)
struct  MapCoord
{
	//GENERATED_BODY()
	//UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 x;
	//UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 y;
};

class SMITH_API MapObject
{
public:
	MapObject(AActor* actor);
	virtual ~MapObject();

	AActor* GetActor()const;

	MapCoord GetCoord()const;
	void SetCoord(MapCoord setterCoord);

private:
	MapCoord myCoord;
	TWeakObjectPtr<AActor> p_actor;
};

class SMITH_API SmithMapManager
{
public:
	SmithMapManager();
	virtual ~SmithMapManager();

	MapCoord GetObjectCorrd(MapObject* Obj);
	void RegistraitonObject(MapObject* Obj);
	void ReleaseObject(MapObject* Obj);
	void UpdateObject(MapObject* Obj, MapCoord);


private:
	FVector const originWorldCoord;
	TArray<MapObject*>Objects;

	MapObject* GetObject(MapObject* Obj);

};
