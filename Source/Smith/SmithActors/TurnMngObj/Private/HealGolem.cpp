// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithActors/TurnMngObj/Public/HealGolem.h"
#include "SmithAttackComponent.h"
#include "SmithPlayerActor.h"

#include "Debug.h"

AHealGolem::AHealGolem()
{
  PrimaryActorTick.bCanEverTick = true;

  m_hp = 3.0f;
  m_atk = 2;
  m_skillCoolTurn = 2;
  m_healPoint = 2;
  m_skillCnt = m_skillCoolTurn;
  target = nullptr;
}

void AHealGolem::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  // コマンドが送れる状態か
  if (IsCommandSendable())
  {
    m_timer += DeltaTime;
  }

  if (m_timer > 0.5f)
  {
    target = PlayerCheck();
    IAttackable* attack = Cast<IAttackable>(target);
    if (Cast<ASmithEnemy>(target) != nullptr && m_skillCnt <= 0)
    {
      SendSkillCommand(this);
    }
    else if (attack != nullptr)
    {
      SendAttackCommand(m_attackComp, attack, AttackHandle{GetName(), m_atk});
      --m_skillCnt;
    }
    else
    {
      // 移動の処理
      m_moveComp->SetTerminusPos(MoveDirection());
      SendMoveCommand(m_moveComp);
    }

    m_timer = 0.0f;
  }
}

void AHealGolem::OnSkill()
{
  MDebug::Log("Skill" + GetName());
  ASmithEnemy* enemy = Cast<ASmithEnemy>(target);
  enemy->OnHeal(m_healPoint);
  m_skillCnt = m_skillCoolTurn;
}