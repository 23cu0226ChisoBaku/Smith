// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithGameOver/GameOverLevel.h"
#include "Debug.h"
#include "Misc/PackageName.h"
#include "Kismet/GameplayStatics.h"

void UGameOverLevel::OpenLevel()
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