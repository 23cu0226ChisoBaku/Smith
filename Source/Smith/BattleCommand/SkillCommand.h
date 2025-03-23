// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_BATTLE_SKILL_COMMAND
#define SMITH_BATTLE_SKILL_COMMAND

#include "IBattleCommand.h"

#include "UObject/WeakInterfacePtr.h"
#include "ISmithAnimator.h"

namespace UE::Smith::Command
{
  template<typename SkillStrategy>
  class SkillCommand final : public IBattleCommand
  {
    public:
      SkillCommand(SkillStrategy Strategy, ISmithAnimator* Animator = nullptr, uint8 SkillSlot = 0u)
        : m_strategy(::MoveTemp(Strategy))
        , m_animator(Animator)
        , m_skillSlot(SkillSlot)
      { }
      virtual ~SkillCommand()
      {
        m_animator.Reset();
      }

    public:

      void Start() override final
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

      void Execute(float DeltaTime) override final
      {
        if (m_animator.IsValid())
        {
          m_animator->UpdateAnimation(DeltaTime);
        }
      }

      void End() override final
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimation(SMITH_ANIM_IDLE);
        }

        m_strategy();
      }

      bool IsFinish() const override final
      {
        return m_animator.IsValid() ? m_animator->IsAnimationFinish() : true;
      }

    private:

      SkillStrategy m_strategy;

      TWeakInterfacePtr<ISmithAnimator> m_animator;

      uint8 m_skillSlot = 0u;

  };
}

#endif