// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_TestTileMove.h"

AGM_TestTileMove::AGM_TestTileMove()
{
  static ConstructorHelpers::FClassFinder<APawn> TileMoveBPClass(TEXT("/Game/BP/BP_TestTileMove"));
  if (TileMoveBPClass.Class != nullptr)
  {
    DefaultPawnClass = TileMoveBPClass.Class;
  }

}