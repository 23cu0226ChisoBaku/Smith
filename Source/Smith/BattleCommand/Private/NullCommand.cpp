// Fill out your copyright notice in the Description page of Project Settings.
/*

NullCommand.cpp

Author : MAI ZHICONG(バクチソウ)

Description : バトルヌルコマンド（何もしないコマンド）

Update History: 2024/12/10		作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "NullCommand.h"

namespace UE::Smith::Command
{
  NullCommand::NullCommand()
  { }

  NullCommand::~NullCommand()
  {
    memset(this, 0, sizeof(*this));
  }

  void NullCommand::Start()
  { }

  void NullCommand::Execute(float deltaTime)
  { }

  void NullCommand::End()
  { }

  bool NullCommand::IsFinish() const
  {
    return true;
  }
}
