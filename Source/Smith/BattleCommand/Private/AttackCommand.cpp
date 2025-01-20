// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCommand.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithAttackComponent.h"
#include "IAttackable.h"
#include "ISmithAnimator.h"
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
      AttackImpl(ICanMakeAttack* attacker, ISmithAnimator* animator)
        : m_attacker(attacker)
        , m_animator(animator)
      { }
      ~AttackImpl()
      {
        m_attacker.Reset();
        m_animator.Reset();
      }

    public:
      void Start()
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimation(SMITH_ANIM_ATTACK);
        }
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

        if (m_animator.IsValid())
        {
          m_animator->UpdateAnimation(deltaTime);
        }
      }
      void End()
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimation(SMITH_ANIM_IDLE);
        }
      }
      bool IsFinish() const
      {
        return m_animator.IsValid() ? m_animator->IsAnimationFinish() : true;
      }
    private:
      TWeakInterfacePtr<ICanMakeAttack> m_attacker;
      TWeakInterfacePtr<ISmithAnimator> m_animator;
  };
  #pragma endregion AttackCommand PImpl
  // end of AttackCommand PImpl

  AttackCommand::AttackCommand(ICanMakeAttack* attacker, IAttackable* target, AttackHandle&& handle, ISmithAnimator* animator)
    : m_attackImpl(nullptr)
  {
    if (attacker != nullptr && ::IsValid(attacker->_getUObject()))
    {
      attacker->SetAttackTarget(target);
      attacker->SetAttackHandle(::MoveTemp(handle));
    }
    m_attackImpl = ::MakeUnique<AttackImpl>(attacker, animator);
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