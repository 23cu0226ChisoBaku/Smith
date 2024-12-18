// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCommand.h"
#include "SmithAttackComponent.h"
#include "IAttackable.h"
#include "AttackHandle.h"
#include "Debug.h"

namespace UE::Smith::Command
{
  AttackCommand::AttackCommand(USmithAttackComponent* atkComp, IAttackable* target, AttackHandle&& handle)
    : m_atkComp(atkComp)
  {
    if (m_atkComp.IsValid())
    {
      m_atkComp->SetAttackTarget(target);
      m_atkComp->SetAttackHandle(::MoveTemp(handle));
    }
  }

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
    if (m_atkComp.IsValid())
    {
      attackStr.Append(m_atkComp->GetName());
      m_atkComp->Attack_Temp();
    }
    else
    {
      attackStr.Append(TEXT("EMPTY OBJECT"));
    }

    attackStr.Append(TEXT(" Attack Command"));

    UE::MLibrary::Debug::Log(attackStr);

  }

  void AttackCommand::End()
  {
    
  }

  bool AttackCommand::IsFinish() const
  {
    return true;
  }

}