// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestTypeErasure.h"

#include "BattleCommand.h"
#include "BattleCommandStrategies.h"

void RunCommand(const UE::Smith::Command::BattleCommand& Command, float DeltaTime)
{
	StartCommand(Command);
	UpdateCommand(Command, DeltaTime);
	TerminateCommand(Command);
}

// Sets default values
ATestTypeErasure::ATestTypeErasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestTypeErasure::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestTypeErasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static float TimeCount;

	TimeCount += DeltaTime;

	if (TimeCount >= TimeInterval)
	{
		UE::Smith::Command::TestCommand cmd{};

		RunCommand(UE::Smith::Command::BattleCommand(cmd), DeltaTime);
		TimeCount = 0.0f;
	}

}

