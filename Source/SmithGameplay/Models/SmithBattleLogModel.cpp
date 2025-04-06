// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleLogModel.h"

FSmithBattleLogModel::FSmithBattleLogModel()
  : m_logModelClass(nullptr)
  , m_name{}
  , m_type(EBattleLogModelType::TypeCount)
{ }

FSmithBattleLogModel FSmithBattleLogModel::CreateModel(UClass* LogModelClass, const FString& Name, EBattleLogModelType Type)
{
  check(LogModelClass != nullptr);

  FSmithBattleLogModel model;
  model.m_logModelClass = LogModelClass;
  model.m_name = Name;
  model.m_type = Type;

  return model;
}

bool FSmithBattleLogModel::IsValid() const
{
  if (   (m_logModelClass == nullptr)
      || (m_name.IsEmpty())
      || (StaticCast<uint8>(m_type) >= (uint8)EBattleLogModelType::TypeCount)
     )
    {
      return false;
    }

  return true;
}


