// Fill out your copyright notice in the Description page of Project Settings.
/*

IdleCommand.h

Author : MAI ZHICONG(バクチソウ)

Description : バトルヌルコマンド（何もしないコマンド）

Update History: 2024/12/10		作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

#ifndef SMITH_BATTLE_IDLE_COMMAND
#define SMITH_BATTLE_IDLE_COMMAND

namespace UE::Smith::Command
{
  ///
  ///	@brief アイドルコマンド
  /// namespace UE::Smith::Command
  ///
  class IdleCommand final
  {
    public:

      IdleCommand(float idleTime = 0.0f)
        : m_idleTimeCnt(0.0f)
        , m_idleTimeInterval(idleTime)
      { }

      ~IdleCommand() 
      { }

    public:

      void Start() { }

      void Update(float DeltaTime)
      { 
        m_idleTimeCnt += DeltaTime;
      }

      void End() { }

      bool IsFinished() const
      {
        return m_idleTimeCnt >= m_idleTimeInterval;
      }

    private:

      float m_idleTimeCnt;
      float m_idleTimeInterval;   // コマンド待機時間
  };
}

#endif
