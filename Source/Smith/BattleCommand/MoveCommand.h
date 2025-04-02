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

#include "UObject/WeakInterfacePtr.h"
#include "ISmithAnimator.h"

#include <type_traits>
#include <concepts>

// コマンドストラテジーコンセプト
// 実装すべきもの：
// void operator()(float) const;              
// explicit operator bool() const noexcept;     // boolキャスト
//  

template<typename Strategy>
concept ConceptMoveStrategy = requires(Strategy Strategy_, float DeltaTime)
{
  {Strategy_(DeltaTime)} -> std::same_as<void>;
  (bool)Strategy_; 
};

namespace UE::Smith::Command
{
  ///
  ///	@brief 移動コマンド
  /// namespace UE::Smith::Command
  ///
  template<ConceptMoveStrategy Strategy>
  class MoveCommand final
  {
    public:
    
      MoveCommand(Strategy Strategy_, ISmithAnimator* Animator = nullptr)
        : m_moveStrategy(::MoveTemp(Strategy_))
        , m_animator(Animator)
      { }

      ~MoveCommand()
      {
        m_animator.Reset();
      }

    public:

      void Start()
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimation(SMITH_ANIM_WALK);
        } 
      } 

      void Update(float DeltaTime)
      {
        // 移動ストラテジーを実行
        m_moveStrategy(DeltaTime);

        if (m_animator.IsValid())
        {
          m_animator->UpdateAnimation(DeltaTime);
        }
      }

      void End()
      {
        if (m_animator.IsValid())
        {
          m_animator->SwitchAnimationDelay(SMITH_ANIM_IDLE, 0.2f);
        }
      }

      bool IsFinished() const
      {
        // 移動ストラテジーが完了したかを調べる
        return (bool)m_moveStrategy;
      }

    private:
      
      Strategy m_moveStrategy;

      TWeakInterfacePtr<ISmithAnimator> m_animator;
  };
}

#endif