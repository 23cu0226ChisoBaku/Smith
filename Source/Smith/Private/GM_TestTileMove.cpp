// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_TestTileMove.h"

namespace TEST_BPCLASS_PATH
{

}

AGM_TestTileMove::AGM_TestTileMove()
{
  static ConstructorHelpers::FClassFinder<APawn> TileMoveBPClass(TEXT("/Game/BP/BP_TestTileMove"));
  if (TileMoveBPClass.Class != nullptr)
  {
    DefaultPawnClass = TileMoveBPClass.Class;
  }
  else
  {
    if(GEngine)
    {
      GEngine->AddOnScreenDebugMessage(-1, 10.f,FColor::Red, TEXT("ajsdiajflkasjdlkasd"));
    }
  }
}