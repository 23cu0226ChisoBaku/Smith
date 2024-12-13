// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCommand.h"
#include "SmithPlayerActor.h"
#include "Debug.h"

namespace UE::Smith::Command
{
  AttackCommand::AttackCommand(ASmithPlayerActor* actor)
    : m_actor(actor)
  {}

  AttackCommand::~AttackCommand()
  {
    memset(this, 0 , sizeof(this));
  }

  void AttackCommand::Execute()
  {
    if (GEngine != nullptr)
    {
      FString attackStr;
      if (m_actor.IsValid())
      {
        attackStr.Append(m_actor->GetName());
      }
      else
      {
        attackStr.Append(TEXT("EMPTY OBJECT"));
      }

      attackStr.AppendChars(TEXT(" Attack Command"), 16);

      GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, attackStr);

      UE::MLibrary::Debug::Log("test");
    }
  }
}