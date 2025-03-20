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

#include "CoreMinimal.h"
#include "IBattleCommand.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class IMoveable;
class ISmithAnimator;

namespace UE::Smith::Command
{
  class SMITH_API MoveCommand final : public IBattleCommand
  {
  public:
    MoveCommand(IMoveable*, ISmithAnimator* = nullptr);
    ~MoveCommand();

  public:
    void Start() override final;
    void Execute(float deltaTime) override final;
    void End() override final;
    bool IsFinish() const override final;

  private:
    /// @brief	移動コマンド実装クラス(pImplイディオム)
    class MoveImpl;
    TUniquePtr<MoveImpl> m_moveImpl;
  };
}

#endif