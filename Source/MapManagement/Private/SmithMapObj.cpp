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

namespace UE::Smith
{
  namespace Map
  {
    bool operator==(const FSmithMapObj& lhs, const FSmithMapObj& rhs)
    {
      return lhs.ReferenceEquals(rhs.GetActor()) && lhs.GetUniqueID() == rhs.GetUniqueID();
    }
    class FSmithMapObj::MapObjImpl
    {
      public:
        MapObjImpl(AActor* actor)
          : m_actor(actor)
          , m_uniqueID({})
        {
          if (::IsValid(m_actor.Get()))
          {
            const FString uniqueStr = m_actor->GetName() + FString::FromInt(m_actor->GetUniqueID());
            m_uniqueID = FGuid{uniqueStr};
          }
        }
        ~MapObjImpl()
        {
          m_actor.Reset();
          m_uniqueID.Invalidate();
        }
        AActor* GetActor() const
        {
          return m_actor.Get();
        }
        FGuid GetUniqueID() const
        {
          return m_uniqueID;
        }
        bool IsValid() const
        {
          return m_actor.IsValid() && m_uniqueID.IsValid();
        }
        bool ReferenceEquals(const AActor* actor) const
        {
          if (!::IsValid(actor) || !IsValid()) [[unlikely]]
          {
            return false;
          }

          return m_actor.Get() == actor;
        }
      private:
        TWeakObjectPtr<AActor> m_actor;
        FGuid m_uniqueID;
    };

    FSmithMapObj::FSmithMapObj(AActor* actor)
      : m_pImpl(::MakeUnique<MapObjImpl>(actor))
    { }

    FSmithMapObj::~FSmithMapObj()
    {
      m_pImpl.Reset();
    }
    FGuid FSmithMapObj::GetUniqueID() const
    {
      return m_pImpl->GetUniqueID();
    }
    AActor* FSmithMapObj::GetActor() const
    {
      return m_pImpl->GetActor();
    }
    bool FSmithMapObj::IsValid() const
    {
      return m_pImpl.IsValid();
    }
    bool FSmithMapObj::ReferenceEquals(const AActor* actor) const
    {
      return m_pImpl->ReferenceEquals(actor);
    }
  }
}
