// MapManager
// ã–ìŒ’“l


#pragma once

#include "CoreMinimal.h"


struct MapCoord
{
	UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 x;
	UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 y;
};

// ‰¼’u‚« UŒ‚”ÍˆÍ‚Æ‚©‚Ìî•ñ‚ğ‚ÂH
struct Format
{

};

class SMITH_API MapObject
{
public:
	MapObject();
	virtual ~MapObject();

	bool Mpve();
	int GetAttack();
	MapCoord myCoord;
};

class SMITH_API SmithMapManager
{
public:
	SmithMapManager();
	virtual ~SmithMapManager();

	//MapCoord 

	
private:
	FVector const originWorldCoord;
};
