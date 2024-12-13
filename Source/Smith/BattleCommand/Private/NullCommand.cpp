// Fill out your copyright notice in the Description page of Project Settings.


#include "NullCommand.h"
#include "Debug.h"

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
    UE::MLibrary::Debug::Log("NULL COMMAND");  
  }
}
