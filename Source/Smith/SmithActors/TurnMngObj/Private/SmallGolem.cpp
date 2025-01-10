// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithActors/TurnMngObj/Public/SmallGolem.h"
#include "SmithAttackComponent.h"
#include "SmithPlayerActor.h"

#include "Debug.h"

ASmallGolem::ASmallGolem()
{
	PrimaryActorTick.bCanEverTick = true;

	m_hp = 5.0f;
	m_atk = 3;
	m_skillCoolTurn = 2;
	m_skillCnt = m_skillCoolTurn;
	target = nullptr;
}

void ASmallGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MDebug::LogWarning("Enemy Tick");
	// コマンドが送れる状態か
	if (IsCommandSendable())
	{
		m_timer += DeltaTime;
		MDebug::LogError("Enemy IsCommandSendable");
	}
	else
	{
		MDebug::LogError("No Sendable");
	}

	if (m_timer > 0.5f)
	{
		target = PlayerCheck();
		IAttackable *attack = Cast<IAttackable>(target);
		// if (attack != nullptr && m_skillCnt <= 0)
		// {
		// 	//SendSkillCommand(this);
		// }
		if (attack != nullptr)
		{
			//SendAttackCommand(m_attackComp, attack, AttackHandle{GetName(), m_atk});
			//--m_skillCnt;
		}
		else
		{
			// 移動の処理
			//m_moveComp->SetTerminusPos(MoveDirection());
			SendMoveCommand(m_moveComp,(UE::Smith::Battle::EMoveDirection)MoveDirection(),1);
		}

		m_timer = 0.0f;
	}
}

void ASmallGolem::OnSkill()
{
	MDebug::Log("Skill" + GetName());
	ASmithPlayerActor *player = Cast<ASmithPlayerActor>(target);
	player->OnAttack(AttackHandle{GetName(), m_atk + 2});
	m_skillCnt = m_skillCoolTurn;
}