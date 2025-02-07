// Fill out your copyright notice in the Description page of Project Settings.


#include "NullCommand.h"

namespace UE::Smith::Command
{
  NullCommand::NullCommand()
  { }

  NullCommand::~NullCommand()
  {
    memset(this, 0, sizeof(*this));
  }

  void NullCommand::Start()
  { }

  void NullCommand::Execute(float deltaTime)
  { }

  void NullCommand::End()
  { }

  bool NullCommand::IsFinish() const
  {
    return true;
  }
}
