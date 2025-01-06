// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMapGeneratePawn.h"
#include "SmithRect.h"
#include "SmithMap.h"
#include "SmithMapBuilder.h"
#include "SmithMapConstructor.h"

#include "MLibrary.h"

// Sets default values
ATestMapGeneratePawn::ATestMapGeneratePawn()
	: map(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MDebug::gLogTime = 600.0f;
}

// Called when the game starts or when spawned
void ATestMapGeneratePawn::BeginPlay()
{
	Super::BeginPlay();
	// using namespace UE::Smith::Map;

	// TSharedPtr<FSmithMapBuilder> builder = ::MakeShared<FSmithMapBuilder>();
	// map = new UE::Smith::Map::FSmithMap();

	// builder->Build(map, blueprint);
	
	// UE::Smith::Map::FSmithRect mapRect = map->GetMap();
	
	// for (uint8 y = 0; y < mapRect.GetHeight(); ++y)
	// {
	// 	FString str{};
	// 	for (uint8 x = 0; x < mapRect.GetWidth(); ++x)
	// 	{
	// 		str.Append(FString::FromInt(mapRect.GetRect(x,y)));
	// 		str.Append(TEXT(" "));
	// 	}
	// 	MDebug::LogWarning(str);
	// }

	// TSharedPtr<FSmithMapConstructor> constructor = ::MakeShared<FSmithMapConstructor>();
	// constructor->ConstructMap(GetWorld(), mapRect, constructionBP);

	// delete map;
}

// Called every frame
void ATestMapGeneratePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestMapGeneratePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

