// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithStaticObj.h"
using namespace UE::MLibrary;

// Sets default values
ASmithStaticObj::ASmithStaticObj()
	: m_arr(TDimension2Array<int32>(10,10))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmithStaticObj::BeginPlay()
{
	Super::BeginPlay();
	m_arr[9][9] = 10;
}

// Called every frame
void ASmithStaticObj::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GEngine != nullptr)
	{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(m_arr[9][9]));
	}

}

