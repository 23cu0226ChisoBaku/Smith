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

#include "CoreMinimal.h"
#include "IBattleCommand.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class ICanMakeAttack;
class IAttackable;
class ISmithAnimator;
// TODO
struct AttackHandle;

namespace UE::Smith::Command
{
  ///
  ///	@brief 攻撃コマンド
  /// namespace UE::Smith::Command
  /// implemented IBattleCommand
  ///
  class SMITH_API AttackCommand final: public IBattleCommand
  {
    public:
      AttackCommand(ICanMakeAttack*, IAttackable*, AttackHandle&&, ISmithAnimator* = nullptr);
      ~AttackCommand();

    public:
      virtual void Start() override;
      virtual void Execute(float deltaTime) override;
      virtual void End() override;
      virtual bool IsFinish() const override;

    private:
      /// @brief 攻撃コマンド実装クラス(pImplイディオム)
      class AttackImpl;
      TUniquePtr<AttackImpl> m_attackImpl;
  };
}

#endif
