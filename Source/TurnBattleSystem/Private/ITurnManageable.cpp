// Fill out your copyright notice in the Description page of Project Settings.


#include "ITurnManageable.h"

// Add default functionality here for any ITurnManageable functions that are not pure virtual.

void ITurnManageable::SetCommandSendable(bool value)
{
  m_bIsCmdSendable = value;
  
  if (!m_bIsCmdSendable && OnTurnPass.IsBound())
  {
    OnTurnPass.Broadcast();
  }
}

void ITurnManageable::SetTurnPriority(ETurnPriority priority)
{
  m_priority = priority;
}

bool ITurnManageable::IsCommandSendable() const
{
  return m_bIsCmdSendable;
}

ETurnPriority ITurnManageable::GetPriority() const
{
  return m_priority;
}