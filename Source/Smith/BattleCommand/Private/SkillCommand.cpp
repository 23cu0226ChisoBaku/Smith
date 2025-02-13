// Fill out your copyright notice in the Description page of Project Settings.

#include "SkillCommand.h"
#include "UObject/WeakInterfacePtr.h"
#include "ICanMakeAttack.h"
#include "IAttackable.h"
#include "AttackHandle.h"
#include "ISmithAnimator.h"

#include "MLibrary.h"

namespace UE::Smith::Command
{
// SkillCommand実装
// SkillCommand PImpl
#pragma region SkillCommand PImpl
  class SkillCommand::SkillImpl
  {
  public:
    SkillImpl(ICanMakeAttack* attacker, IAttackable* target, AttackHandle handle, ISmithAnimator* animator, uint8 skillSlot)
    {
      if (IS_UINTERFACE_VALID(attacker))
      {
        attacker->SetAttackTarget(target);
        attacker->SetAttackHandle(::MoveTemp(handle));
      }

      m_attacker = attacker;
      m_animator = animator;
      m_skillSlot = skillSlot;
    }
    ~SkillImpl()
    { 
      m_attacker.Reset();
      m_animator.Reset();
    }

  public:
    void Start()
    {
      if (m_animator.IsValid())
      {
        switch (m_skillSlot)
        {
          case 1:
          {
            m_animator->SwitchAnimation(SMITH_ANIM_SKILL_ONE);
          }
          break;
          case 2:
          {
            m_animator->SwitchAnimation(SMITH_ANIM_SKILL_TWO);
          }
          break;
          case 3:
          {
            m_animator->SwitchAnimation(SMITH_ANIM_SKILL_THREE);
          }
          break;
          default:
          {
            m_animator->SwitchAnimation(SMITH_ANIM_IDLE);
          }
        }
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
      if (m_animator.IsValid())
      {
        m_animator->SwitchAnimation(SMITH_ANIM_IDLE);
      }
      if (m_attacker.IsValid())
      {
        m_attacker->Attack();
      }
    }
    bool IsFinish() const
    {
      return m_animator.IsValid() ? m_animator->IsAnimationFinish() : true;
    }

  private:
    TWeakInterfacePtr<ICanMakeAttack> m_attacker;
    TWeakInterfacePtr<ISmithAnimator> m_animator;
    uint8 m_skillSlot;
  };
#pragma endregion MoveCommand PImpl
  // end of MoveCommand PImpl

  SkillCommand::SkillCommand(ICanMakeAttack* attacker, IAttackable* target, AttackHandle handle, ISmithAnimator* animator, uint8 skillSlot)
      : m_skillImpl(::MakeUnique<SkillImpl>(attacker, target, handle, animator, skillSlot))
  { }

  SkillCommand::~SkillCommand()
  {
    m_skillImpl.Reset();
    memset(this, 0, sizeof(*this));
  }

  void SkillCommand::Start()
  {
    m_skillImpl->Start();
  }

  void SkillCommand::Execute(float deltaTime)
  {
    m_skillImpl->Update(deltaTime);
  }

  void SkillCommand::End()
  {
    m_skillImpl->End();
  }

  bool SkillCommand::IsFinish() const
  {
    return m_skillImpl->IsFinish();
  }
}