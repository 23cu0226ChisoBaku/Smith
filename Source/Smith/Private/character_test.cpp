// Fill out your copyright notice in the Description page of Project Settings.


#include "character_test.h"
#include "MapObject.h"

// Sets default values
Acharacter_test::Acharacter_test()
	:mapObj()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void Acharacter_test::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void Acharacter_test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Acharacter_test::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Move", this, &Acharacter_test::move);

}

void Acharacter_test::move(float value) {
	uint8 x = 0;
	uint8 y = 0;

	switch ((uint8)value)
	{
	case 1:		// w
		x = 0;
		y = 1;
		break;
	case 2:		// a
		x = -1;
		y = 0;
		break;
	case 3:		// s
		x = 0;
		y = -1;
		break;
	case 4:		//d
		x = 1;
		y = 0;
		break;
	default:
		x = 0;
		y = 0;
		break;
	}
	mapObj.SetCoord(MapCoord(x, y));
}

//void Acharacter_test::UpdateMove()
//{
//	MapCoord currentcoord = mapObj.GetCoord();
//	
//	const FVector currentlocation = GetActorLocation();
//
//
//
//	SetActorLocation()
//}

