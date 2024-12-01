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
	type[0] = EFFECT;
	type[1] = EFFECT;
	type[2] = EFFECT;
	type[3] = EFFECT;
	type[4] = EFFECT;
	type[5] = EFFECT;
	type[6] = NO_EFFECT;
	type[7] = CENTER_NO_EFFECT;
	type[8] = NO_EFFECT;
	
	m_format.SetupFormat(type, 9 * sizeof(ESmithFormatType), 3, 3);
	const auto& test = m_format.GetFormatArray();

}

// Called every frame
void ASmithStaticObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

