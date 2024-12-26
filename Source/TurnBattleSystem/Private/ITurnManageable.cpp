// Fill out your copyright notice in the Description page of Project Settings.


#include "ITurnManageable.h"

// Add default functionality here for any ITurnManageable functions that are not pure virtual.

class ITurnManageable::TurnManageImpl
{
  public:
    TurnManageImpl()
      : m_priority(ETurnPriority::PlayerSelf)
      , m_bIsCmdSendable(false)
    {

    }
    ~TurnManageImpl() = default;
  public:
    void SetCmdSendable(bool value)
    {
      m_bIsCmdSendable = value;
    }
    void SetTurnPriority(ETurnPriority priority)
    {
      m_priority = priority;
    }
    bool IsCmdSendable() const
    {
      return m_bIsCmdSendable;
    }
    ETurnPriority GetPriority() const
    {
      return m_priority;
    }
  private:
    ETurnPriority m_priority;
    uint8 m_bIsCmdSendable : 1;
};

ITurnManageable::ITurnManageable()
  : m_pImpl(new TurnManageImpl())
  {} 

ITurnManageable::~ITurnManageable()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

void ITurnManageable::SetCommandSendable(bool value)
{
  m_pImpl->SetCmdSendable(value);
}

void ITurnManageable::SetTurnPriority(ETurnPriority priority)
{
  m_pImpl->SetTurnPriority(priority);
}

bool ITurnManageable::IsCommandSendable() const
{
  return m_pImpl->IsCmdSendable();
}

ETurnPriority ITurnManageable::GetPriority() const
{
  return m_pImpl->GetPriority();
}