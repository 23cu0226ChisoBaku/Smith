// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveCommand.h"
#include "SmithPlayerActor.h"

MoveCommand::MoveCommand(ASmithPlayerActor* actor)
  : m_actor(actor)
{}

MoveCommand::~MoveCommand()
{
  memset(this, 0, sizeof(this));
}

void MoveCommand::Execute()
{
  if (GEngine != nullptr)
  {
    FString moveStr;
    if (m_actor.IsValid())
    {
      moveStr.Append(m_actor->GetName());
    }
    else
    {
      moveStr.Append(TEXT("EMPTY OBJECT"));
    }

    moveStr.AppendChars(TEXT(" Move Command"), 16);

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, moveStr);
  }
}
