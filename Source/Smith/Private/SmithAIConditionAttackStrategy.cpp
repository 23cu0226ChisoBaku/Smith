// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithAIConditionAttackStrategy.h"
#include "ICommandMediator.h"
#include "ICanMakeAttack.h"
#include "AttackHandle.h"
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

  TQueue<FConditionHandle> queue;
  FConditionHandle tempHandle;
  bool isExecuted = false;
  while (m_conditions.Dequeue(tempHandle))
  {
    queue.Enqueue(tempHandle);
    // 条件を満たしているか
    if (tempHandle.Condition.Execute())
    {
      // 攻撃
      if (!m_attackFormatTables.Contains(tempHandle.Name))
      {
        MDebug::LogError("Not Found Key");
        continue;
      }

      const auto& format = m_attackFormatTables[tempHandle.Name];
      if (!format.IsValid())
      {
        MDebug::LogError("Format Invalid");
        continue;
      }

      for (uint8 i = 0u; i < 4u; ++i)
      {
        EDirection atkDir = StaticCast<EDirection>(i * 2u);

        bool success = m_mediator->SendAttackCommand(GetOwner(), m_attacker.Get(), atkDir, *m_attackFormatTables[tempHandle.Name], AttackHandle{GetName(), m_atk}, false);
        if (success)
        {
          isExecuted = true;
          break;
        }
      }
    }
  }

  // 残った要素を代入
  while (m_conditions.Dequeue(tempHandle))
  {
    queue.Enqueue(tempHandle);
  }

  // 要素を元に戻す
  while (queue.Dequeue(tempHandle))
  {
    m_conditions.Enqueue(tempHandle);
  }

  return isExecuted;
}