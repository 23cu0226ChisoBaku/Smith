// Fill out your copyright notice in the Description page of Project Settings.


#include "NullCommand.h"

namespace UE::Smith::Command
{
  NullCommand::NullCommand()
  {
  }

  NullCommand::~NullCommand()
  {
  }

  void NullCommand::Execute()
  {
    if (GEngine != nullptr)
    {
      GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("NULL COMMAND"));
    }
  }
}
