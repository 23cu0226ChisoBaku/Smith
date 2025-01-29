// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithGameOver/gameover_test.h"
#include "Debug.h"
#include "Misc/PackageName.h"
#include "Kismet/GameplayStatics.h"

void Ugameover_test::TriggerEvent()
{
	FString levelpath = TEXT(" / Game / TestLevel / ueno / GameOver_test");
	if (FPackageName::DoesPackageExist(levelpath))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*levelpath));
	}
	else
	{
		MDebug::LogWarning("gameoverlevel not found");
	}
}

