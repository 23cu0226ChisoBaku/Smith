// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_BATTLE_SKILL_COMMAND
#define SMITH_BATTLE_SKILL_COMMAND

#include "UObject/WeakInterfacePtr.h"
#include "ISmithAnimator.h"

// コマンドストラテジーコンセプト
// 実装すべきもの：
// void operator()() const;   
template<typename Strategy>
concept ConceptSkillStrategy = requires(Strategy Strategy_)
{
  {Strategy_()} -> std::same_as<void>;
};

namespace UE::Smith::Command
{
  ///
  ///	@brief スキルコマンド
  /// namespace UE::Smith::Command
  ///
  template<ConceptSkillStrategy SkillStrategy>
  class SkillCommand final
  {
    public:

      SkillCommand(SkillStrategy Strategy, ISmithAnimator* Animator = nullptr, uint8 SkillSlot = 0u)
        : m_strategy(::MoveTemp(Strategy))
        , m_animator(Animator)
        , m_skillSlot(SkillSlot)
      { }

      ~SkillCommand()
      {
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

      void Update(float DeltaTime)
      {
        if (m_animator.IsValid())
        {
          m_animator->UpdateAnimation(DeltaTime);
        }
      }

      void End()
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimation(SMITH_ANIM_IDLE);
        }

        // ストラテジーを実行する
        m_strategy();
      }

      bool IsFinished() const
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