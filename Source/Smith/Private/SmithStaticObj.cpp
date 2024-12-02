// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithStaticObj.h"
#include "FormatTransformer.h"
using namespace UE::Smith::Battle;
using namespace UE::MLibrary::MDataStructure;

// Sets default values
ASmithStaticObj::ASmithStaticObj()
	: m_mapCoord({})
	, m_format()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASmithStaticObj::BeginPlay()
{
	Super::BeginPlay();
	ESmithFormatType type[9];
	type[0] = ESmithFormatType::EFFECT;
	type[1] = ESmithFormatType::EFFECT;
	type[2] = ESmithFormatType::EFFECT;
	type[3] = ESmithFormatType::EFFECT;
	type[4] = ESmithFormatType::EFFECT;
	type[5] = ESmithFormatType::EFFECT;
	type[6] = ESmithFormatType::NO_EFFECT;
	type[7] = ESmithFormatType::CENTER_NO_EFFECT;
	type[8] = ESmithFormatType::NO_EFFECT;
	
	m_format.SetupFormat(type, 9 * sizeof(ESmithFormatType), 3, 3);
	const auto& test = m_format.GetFormatArray();
	if (GEngine)
	{
		for (auto ENUM : test)
		{
			FString x = FString::FromInt(StaticCast<uint8>(ENUM));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, x);
		}
	}

	FMapCoord cnCoordTemp = { 0, 9 };

  auto mapCoordArr = FFormatTransformer::FormatToMapCoord(m_format, cnCoordTemp);

	for (uint64 y = 0 ; y < mapCoordArr.Row(); ++y)
	{
		FString rowString = {};
		for (uint64 x = 0 ; x < mapCoordArr.Column(); ++x)
		{
			rowString += "{ ";
			rowString += FString::FromInt(mapCoordArr[y][x].x);
			rowString += " , ";
			rowString += FString::FromInt(mapCoordArr[y][x].y);
			rowString += " } ";
		}
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, rowString);
		}
	}

}

// Called every frame
void ASmithStaticObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

