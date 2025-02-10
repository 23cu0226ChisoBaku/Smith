// Fill out your copyright notice in the Description page of Project Settings.
/*

AttackCommand.cpp

Author : MAI ZHICONG(バクチソウ)

Description : バトル攻撃コマンド

Update History: 2024/12/10		作成
                2025/01/20		アニメーション制御追加
                2025/02/10		追記	AttackHandleをFAttackHandleに変更する予定

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "AttackCommand.h"
#include "IAttackable.h"
#include "ICanMakeAttack.h"
#include "ISmithAnimator.h"
#include "AttackHandle.h"
#include "UObject/WeakInterfacePtr.h"

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
        if (m_animator.IsValid())
        {
          m_animator->UpdateAnimation(deltaTime);
        }
      }
      void End()
      {
        if (m_attacker.IsValid())
        {
          m_attacker->Attack();
        }
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
      /// 攻撃する実装
      TWeakInterfacePtr<ICanMakeAttack> m_attacker;
      // アニメーション制御
      TWeakInterfacePtr<ISmithAnimator> m_animator;
  };
  #pragma endregion AttackCommand PImpl
  // end of AttackCommand PImpl

  AttackCommand::AttackCommand(ICanMakeAttack* attacker, IAttackable* target, AttackHandle&& handle, ISmithAnimator* animator)
    : m_attackImpl(nullptr)
  {
    // TODO
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