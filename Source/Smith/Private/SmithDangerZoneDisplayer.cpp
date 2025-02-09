// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithDangerZoneDisplayer.h"

// Sets default values
ASmithDangerZoneDisplayer::ASmithDangerZoneDisplayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASmithDangerZoneDisplayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASmithDangerZoneDisplayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Dispose();
}

// Called every frame
void ASmithDangerZoneDisplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmithDangerZoneDisplayer::Dispose()
{
	for(int32 i = 0; i < m_dangerZonePlane.Num(); ++i)
	{
		if (::IsValid(m_dangerZonePlane[i]))
		{
			m_dangerZonePlane[i]->Destroy();
		}
	}

	m_dangerZonePlane.Empty();
}

void ASmithDangerZoneDisplayer::SetupDisplayLocations(const TArray<FVector>& locations)
{
	if (PlaneObjectSubclass == nullptr)
	{
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	for (const auto& location : locations)
	{
		AActor* dangerZone = world->SpawnActor<AActor>(PlaneObjectSubclass, location, FRotator::ZeroRotator);
		m_dangerZonePlane.Emplace(dangerZone);
	}
}

