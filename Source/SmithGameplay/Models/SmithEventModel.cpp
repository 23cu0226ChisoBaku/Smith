// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithEventModel.h"

FSmithEventModel::FSmithEventModel()
  : m_eventClass(nullptr)
  , m_succeededMsg{}
  , m_failedMsg{}
{}

FSmithEventModel FSmithEventModel::CreateModel(UClass* EventClass, const FString& SucceededMsg, const FString& FailedMsg)
{
  check(EventClass != nullptr);

  FSmithEventModel model;
  model.m_eventClass = EventClass;
  model.m_succeededMsg = SucceededMsg;
  model.m_failedMsg = FailedMsg;

  return model;
}

bool FSmithEventModel::IsValid() const
{
  if (m_eventClass == nullptr)
  {
    return false;
  }

  return true;
}