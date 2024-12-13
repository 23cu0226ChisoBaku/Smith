// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveCommand.h"
#include "SmithPlayerActor.h"
#include "SmithMoveComponent.h"

#include "Debug.h"

namespace UE::Smith::Command
{
  MoveCommand::MoveCommand(USmithMoveComponent* moveComp)
    : m_moveComp(moveComp)
  {}

  MoveCommand::~MoveCommand()
  {
    memset(this, 0, sizeof(this));
  }

  void MoveCommand::Execute()
  {
    FString moveStr{};
    if (m_moveComp.IsValid())
    {
      moveStr.Append(m_moveComp->GetName());
    }
    else
    {
      moveStr.Append(TEXT("EMPTY OBJECT"));
    }

    moveStr.AppendChars(TEXT(" Move Command"), 16);
    UE::MLibrary::Debug::Log(moveStr);
  }
}