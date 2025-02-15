// Fill out your copyright notice in the Description page of Project Settings.
/*

IdleCommand.cpp

Author : MAI ZHICONG(バクチソウ)

Description : バトルヌルコマンド（何もしないコマンド）

Update History: 2024/12/10		作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "IdleCommand.h"

namespace UE::Smith::Command
{
  class IdleCommand::IdleImpl
  {
    public:
      IdleImpl(float idleTime)
        : m_idleTimeInterval(idleTime)
        , m_idleTimeCnt(0.0f)
      { }
      ~IdleImpl()
      { }
    public:
      void Update(float deltaTime)
      {
        m_idleTimeCnt += deltaTime;
      }
      bool IsFinish() const
      {
        return m_idleTimeCnt >= m_idleTimeInterval;
      }
    private:
      float m_idleTimeInterval;
      float m_idleTimeCnt;
  };
  IdleCommand::IdleCommand(float idleTime)
    : m_pImpl(::MakeUnique<IdleImpl>(idleTime))
  { }

  IdleCommand::~IdleCommand()
  {
    m_pImpl.Reset();
    memset(this, 0, sizeof(*this));
  }

  void IdleCommand::Start()
  { }

  void IdleCommand::Execute(float deltaTime)
  { 
    m_pImpl->Update(deltaTime);
  }

  void IdleCommand::End()
  { }

  bool IdleCommand::IsFinish() const
  {
    return m_pImpl->IsFinish();
  }
}
