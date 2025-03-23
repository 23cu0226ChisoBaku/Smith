// Fill out your copyright notice in the Description page of Project Settings.
/*

MoveCommand.h

Author : MAI ZHICONG(バクチソウ)

Description : バトル移動コマンド

Update History: 2024/12/10		作成
                2025/01/20		アニメーション制御追加

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_BATTLE_MOVE_COMMAND
#define SMITH_BATTLE_MOVE_COMMAND

#include "IBattleCommand.h"

#include "UObject/WeakInterfacePtr.h"
#include "ISmithAnimator.h"

namespace UE::Smith::Command
{
  template<typename MoveStrategy>
  class MoveCommand final : public IBattleCommand
  {
    public:
    
      MoveCommand(MoveStrategy Strategy, ISmithAnimator* Animator = nullptr)
        : m_moveStrategy(::MoveTemp(Strategy))
        , m_animator(Animator)
      { }

      ~MoveCommand()
      {
        m_animator.Reset();
      }

    public:

      void Start() override final
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimation(SMITH_ANIM_WALK);
        } 
      } 

      void Execute(float deltaTime) override final
      {
        m_moveStrategy(deltaTime);

        if (m_animator.IsValid())
        {
          m_animator->UpdateAnimation(deltaTime);
        }
      }

      void End() override final
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimationDelay(SMITH_ANIM_IDLE, 0.2f);
        }
      }

      bool IsFinish() const override final
      {
        return (bool)m_moveStrategy;
      }

    private:
      
      MoveStrategy m_moveStrategy;

      TWeakInterfacePtr<ISmithAnimator> m_animator;
  };
}

#endif