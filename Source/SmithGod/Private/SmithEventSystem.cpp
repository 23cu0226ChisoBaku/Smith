// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEventSystem.h"
#include "ICanSetOnMap.h"
#include "ISmithMapEvent.h"
#include "IEventTriggerable.h"
#include "MLibrary.h"

USmithEventSystem::USmithEventSystem(const FObjectInitializer& ObjectInitializer)
  : Super(ObjectInitializer)
  , m_eventHandleContainer{}
{ }

USmithEventSystem::SmithEventHandle::SmithEventHandle(ICanSetOnMap* receiver, ISmithMapEvent* event)
  : EventReceiver(receiver)
  , Event(event)
{ }

void USmithEventSystem::BeginDestroy()
{
  m_eventHandleContainer.Empty();
  Super::BeginDestroy();
}

void USmithEventSystem::RegisterMapEvent(ICanSetOnMap* receiver, ISmithMapEvent* event)
{
  if (!IS_UINTERFACE_VALID(receiver) || !IS_UINTERFACE_VALID(event))
  {
    return;
  }

  m_eventHandleContainer.Emplace(SmithEventHandle(receiver, event));
}

void USmithEventSystem::ExecuteEvent()
{
  int32 idx = 0;

  while(idx < m_eventHandleContainer.Num())
  {
    auto& eventHandle = m_eventHandleContainer[idx];

    if (!eventHandle.EventReceiver.IsValid() || !eventHandle.Event.IsValid())
    {
      m_eventHandleContainer.RemoveAt(idx);
      continue;
    }

    if (!eventHandle.Event->IsDisposed())
    {
      eventHandle.Event->TriggerEvent(eventHandle.EventReceiver.Get());
      if (eventHandle.Event.IsValid() && eventHandle.Event->IsDisposed())
      {
        eventHandle.Event->DiscardEvent();
      }
    }

    ++idx;
  }

  Reset();
}

bool USmithEventSystem::IsEventInStock() const
{
  return m_eventHandleContainer.Num() > 0;
}

void USmithEventSystem::Reset()
{
  m_eventHandleContainer.Reset();
}
