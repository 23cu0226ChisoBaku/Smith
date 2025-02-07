// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithAIConditionAttackStrategy.h"
#include "ICommandMediator.h"
#include "ICanMakeAttack.h"
#include "ISmithBattleLogger.h"
#include "AttackHandle.h"
#include "Direction.h"
#include "MLibrary.h"

void USmithAIConditionAttackStrategy::Initialize(ICanMakeAttack *attacker, ICommandMediator *mediator, int32 attackPower)
{
  m_attacker = attacker;
  m_mediator = mediator;
  m_atk = attackPower;
}

void USmithAIConditionAttackStrategy::ConditionResgister(const FString &name, const UDataTable *formatTable, const TDelegate<bool(void)> &condition)
{
  if (condition.IsBound())
  {
    RegisterAttackFormat(name, formatTable);
    FConditionHandle handle;
    handle.Name = name;
    handle.Condition = condition;
    m_conditions.Enqueue(handle);
  }
  else
  {
    MDebug::LogError("Condition is not bound");
  }
}

bool USmithAIConditionAttackStrategy::executeImpl()
{
  if (!m_mediator.IsValid() || !m_attacker.IsValid())
  {
    MDebug::LogError("not initialize -- attack strategy");
    return false;
  }

  bool isExecuted = false;
  FConditionHandle* curtConditionAttatkHandle = m_conditions.Peek();
  if (curtConditionAttatkHandle == nullptr)
  {
    return false;
  }

  // 条件を満たしているか
  if (curtConditionAttatkHandle->Condition.Execute())
  {
    FConditionHandle curtHandleInstance{};
    m_conditions.Dequeue(curtHandleInstance);
    isExecuted = true;
    // 攻撃
    if (!m_attackFormatTables.Contains(curtHandleInstance.Name))
    {
      MDebug::LogError("Not Found Key");
      m_conditions.Enqueue(curtHandleInstance);
      return false;
    }

    const TSharedPtr<UE::Smith::Battle::FSmithCommandFormat>& format = m_attackFormatTables[curtHandleInstance.Name];
    if (!format.IsValid())
    {
      MDebug::LogError("Format Invalid");
      m_conditions.Enqueue(curtHandleInstance);
      return false;
    }

    // TODO add by Mai
    ISmithBattleLogger* logger = Cast<ISmithBattleLogger>(GetOwner());

    // 南に向けて攻撃
    m_mediator->SendAttackCommand(GetOwner(), m_attacker.Get(), EDirection::South, *format, AttackHandle{logger, m_atk});
    m_conditions.Enqueue(curtHandleInstance);
    
  }
  
  return isExecuted;
}