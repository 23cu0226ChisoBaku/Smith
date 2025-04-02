// Fill out your copyright notice in the Description page of Project Settings.
/*

AttackCommand.h

Author : MAI ZHICONG(バクチソウ)

Description : バトル攻撃コマンド

Update History: 2024/12/10		作成
                2025/01/20		アニメーション制御追加
                2025/02/10		追記	AttackHandleをFAttackHandleに変更する予定

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_BATTLE_ATTACK_COMMAND
#define SMITH_BATTLE_ATTACK_COMMAND

#include "UObject/WeakInterfacePtr.h"
#include "ISmithAnimator.h"

#include <concepts>
#include <type_traits>

// コマンドストラテジーコンセプト
// 実装すべきもの：
// void operator()() const;   
template<typename Strategy>
concept ConceptAttackStrategy = requires(Strategy Strategy_)
{
  {Strategy_()} -> std::same_as<void>;
};

namespace UE::Smith::Command
{
  ///
  ///	@brief 攻撃コマンド
  /// namespace UE::Smith::Command
  ///
  template<ConceptAttackStrategy AttackStrategy>
  class AttackCommand final
  {
    public:

      AttackCommand(AttackStrategy Strategy, ISmithAnimator* Animator = nullptr)
        : m_strategy(Strategy)
        , m_animator(Animator)
      { }

      ~AttackCommand()
      {
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

        // ストラテジーを実行
        m_strategy();
      }

      bool IsFinished() const
      {
        return m_animator.IsValid() ? m_animator->IsAnimationFinish() : true;
      }

    private:
      
      AttackStrategy m_strategy;

      TWeakInterfacePtr<ISmithAnimator> m_animator;
  };
}

#endif
