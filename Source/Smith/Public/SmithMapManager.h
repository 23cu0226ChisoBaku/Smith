// MapManager
// ��쌒�l


#pragma once

#include "CoreMinimal.h"


struct MapCoord
{
	UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 x;
	UPROPERTY(EditinstanceOnly, BluePrintReadWrite)
	uint8 y;
};

// ���u�� �U���͈͂Ƃ��̏������H
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
