// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCommand.h"
#include "SmithPlayerActor.h"
#include "SmithMoveComponent.h"
#include "Debug.h"

namespace UE::Smith::Command
{
  AttackCommand::AttackCommand(USmithMoveComponent* moveComp)
    : m_moveComp(moveComp)
  {}

  AttackCommand::~AttackCommand()
  {
    memset(this, 0 , sizeof(this));
  }

  void AttackCommand::Start()
  {

  }

  void AttackCommand::Execute(float deltaTime)
  {
    FString attackStr{};
    if (m_moveComp.IsValid())
    {
      attackStr.Append(m_moveComp->GetName());
    }
    else
    {
      attackStr.Append(TEXT("EMPTY OBJECT"));
    }

    attackStr.AppendChars(TEXT(" Attack Command"), 16);

    UE::MLibrary::Debug::Log(attackStr);

  }

  void AttackCommand::End()
  {
    
  }
}