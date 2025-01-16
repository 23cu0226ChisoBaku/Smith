// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCommand.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithAttackComponent.h"
#include "IAttackable.h"
#include "AttackHandle.h"
#include "Debug.h"

namespace UE::Smith::Command
{
  // 実装
  // AttackCommand PImpl
  #pragma region AttackCommand PImpl
  class AttackCommand::AttackImpl
  {
    public:
      AttackImpl(ICanMakeAttack* attacker)
        : m_attacker(attacker)
      {

      }
      ~AttackImpl()
      {
        m_attacker.Reset();
      }

    public:
      void Start()
      {

      }
      void Update(float deltaTime)
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
      void End()
      {

      }
      bool IsFinish() const
      {
        return true;
      }
    private:
      TWeakInterfacePtr<ICanMakeAttack> m_attacker;
  };
  #pragma endregion AttackCommand PImpl
  // end of AttackCommand PImpl

  AttackCommand::AttackCommand(ICanMakeAttack* attacker, IAttackable* target, AttackHandle&& handle)
    : m_attackImpl(nullptr)
  {
    if (attacker != nullptr && ::IsValid(attacker->_getUObject()))
    {
      attacker->SetAttackTarget(target);
      attacker->SetAttackHandle(::MoveTemp(handle));
    }
    m_attackImpl = ::MakeUnique<AttackImpl>(attacker);
  }

  AttackCommand::~AttackCommand()
  {
    m_attackImpl.Reset();
    memset(this, 0 , sizeof(*this));
  }

  void AttackCommand::Start()
  {
    m_attackImpl->Start();
  }

  void AttackCommand::Execute(float deltaTime)
  {
    m_attackImpl->Update(deltaTime);
  }

  void AttackCommand::End()
  {
    m_attackImpl->End();
  }

  bool AttackCommand::IsFinish() const
  {
    return m_attackImpl->IsFinish();
  }
}