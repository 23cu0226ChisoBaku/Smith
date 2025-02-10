// Fill out your copyright notice in the Description page of Project Settings.
/*

NullCommand.h

Author : MAI ZHICONG(バクチソウ)

Description : バトルヌルコマンド（何もしないコマンド）

Update History: 2024/12/10		作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_BATTLE_NULL_COMMAND
#define SMITH_BATTLE_NULL_COMMAND

#include "CoreMinimal.h"
#include "IBattleCommand.h"

namespace UE::Smith::Command
{
  ///
  ///	@brief ヌルコマンド
  /// namespace UE::Smith::Command
  /// implemented IBattleCommand
  ///
  class SMITH_API NullCommand final: public IBattleCommand
  {
    public:
      NullCommand();
      ~NullCommand();

    public:
      void Start() override final;
      void Execute(float deltaTime) override final;
      void End() override final;
      bool IsFinish() const override final;
  };
}

#endif
