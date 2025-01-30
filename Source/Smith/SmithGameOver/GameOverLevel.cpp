// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithGameOver/GameOverLevel.h"
#include "Debug.h"
#include "Misc/PackageName.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGameOverLevel::AGameOverLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameOverLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameOverLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameOverLevel::LoadLevel()
{
	FString levelpath = TEXT("/Game/TestLevel/ueno/GameOver_test");
	if (FPackageName::DoesPackageExist(levelpath))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*levelpath));
	}
	else
	{
		MDebug::LogWarning("gameoverlevel not found");
	}
}