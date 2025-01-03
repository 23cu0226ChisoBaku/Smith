// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGenerateGameMode_Test.h"

AMapGenerateGameMode_Test::AMapGenerateGameMode_Test()
{
  static ConstructorHelpers::FClassFinder<APawn> PlayerBP(TEXT("/Game/BP/BP_TestMapGeneratePawn"));
  if (PlayerBP.Class != nullptr)
  {
    DefaultPawnClass = PlayerBP.Class;
  }
}
