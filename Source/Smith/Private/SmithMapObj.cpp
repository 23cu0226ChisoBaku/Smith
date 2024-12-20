// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith Map Object

Author : MAI ZHICONG

Description : Map Object(Use by MapManager)

Update History: 2024/11/25 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#include "SmithMapObj.h"

namespace Smith
{
  FSmithMapObj::FSmithMapObj(AActor* const actor)
    : m_uniqueID()
    , m_actor(actor)
  { 
    if (::IsValid(actor))
    {
      const FString uniqueStr = actor->GetName() + FString::FromInt(actor->GetUniqueID());
      const auto guid = FGuid(uniqueStr);
      m_uniqueID = guid;
    }
  }

  FSmithMapObj::~FSmithMapObj()
  {
    m_uniqueID.Invalidate();
    m_actor.Reset();
  }

  FGuid FSmithMapObj::GetUniqueID() const
  {
    return m_uniqueID;
  }

  AActor* FSmithMapObj::GetActor() const
  {
    if (!m_actor.IsValid())
    {
      return nullptr;
    }
    else [[likely]]
    {
      return m_actor.Get();
    }
  }

  bool FSmithMapObj::IsValid() const
  {
    return m_uniqueID.IsValid() && m_actor.IsValid();
  }

  bool FSmithMapObj::ReferenceEquals(AActor* const actor) const
  {
    if (!::IsValid(actor)) [[unlikely]]
    {
      return false;
    }

    if (!IsValid())
    {
      return false;
    }

    return m_actor.Get() == actor;
  }
}
