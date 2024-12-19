// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCommand.h"
#include "SmithAttackComponent.h"
#include "IAttackable.h"
#include "AttackHandle.h"
#include "Debug.h"

namespace UE::Smith::Command
{
  AttackCommand::AttackCommand(ICanMakeAttack* attacker, IAttackable* target, AttackHandle&& handle)
    : m_attacker(attacker)
  {
    if (m_attacker.IsValid())
    {
      m_attacker->SetAttackTarget(target);
      m_attacker->SetAttackHandle(::MoveTemp(handle));
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
    if (m_attacker.IsValid())
    {
      attackStr.Append(m_attacker->_getUObject()->GetName());
      m_attacker->Attack();
    }
    else
    {
      attackStr.Append(TEXT("EMPTY OBJECT"));
    }

    attackStr.Append(TEXT(" Attack"));

    //UE::MLibrary::Debug::Log(attackStr);

  }

  void AttackCommand::End()
  {
    
  }

  bool AttackCommand::IsFinish() const
  {
    return true;
  }

}