// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveCommand.h"
#include "SmithPlayerActor.h"
#include "SmithMoveComponent.h"
#include "IMoveable.h"

#include "Debug.h"

namespace UE::Smith::Command
{
  MoveCommand::MoveCommand(IMoveable* move)
    : m_move(move)
  {}

  MoveCommand::~MoveCommand()
  {
    memset(this, 0, sizeof(this));
  }

  void MoveCommand::Start()
  {

  }

  void MoveCommand::Execute(float deltaTime)
  {
    FString moveStr{};
    if (m_move.IsValid())
    {
      moveStr.Append(m_move->_getUObject()->GetName());
      m_move->Move();
    }
    else
    {
      moveStr.Append(TEXT("EMPTY OBJECT"));
    }

    moveStr.Append(TEXT(" Move Command"));
    //UE::MLibrary::Debug::Log(moveStr);
  }

  void MoveCommand::End()
  {
    
  }

  bool MoveCommand::IsFinish() const
  {
    return true;
  }

}