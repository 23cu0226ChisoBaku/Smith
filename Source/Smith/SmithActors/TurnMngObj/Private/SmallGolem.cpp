 //// Fill out your copyright notice in the Description page of Project Settings.

 //#include "SmithActors/TurnMngObj/Public/SmallGolem.h"
 //#include "SmithAttackComponent.h"
 //#include "SmithPlayerActor.h"
 //#include "SmithCommandFormat.h"
 //#include "FormatType.h"

 //#include "Debug.h"

 //ASmallGolem::ASmallGolem()
 //{
 //	PrimaryActorTick.bCanEverTick = true;

 //	m_hp = 5.0f;
 //	m_atk = 3;
 //	m_skillCoolTurn = 2;
 //	m_skillCnt = m_skillCoolTurn;
 //	target = nullptr;
 //}

 //void ASmallGolem::Tick(float DeltaTime)
 //{
 //	Super::Tick(DeltaTime);

 //	// コマンドが送れる状態か
 //	if (IsCommandSendable())
 //	{
 //		m_timer += DeltaTime;
 //	}

 //	if (m_timer > 0.5f)
 //	{
 //		target = PlayerCheck();
 //		IAttackable *attack = Cast<IAttackable>(target);
 //		// if (attack != nullptr && m_skillCnt <= 0)
 //		// {
 //		// 	//SendSkillCommand(this);
 //		// }
 //		if (attack != nullptr)
 //		{
 //					UE::Smith::Battle::FSmithCommandFormat formatTest;

 //		ESmithFormatType* type = new ESmithFormatType[9];
 //		for (int32 i = 0; i < 9; ++i)
 //		{
 //			if (i != 4)
 //			{
 //				type[i] = ESmithFormatType::EFFECT;
 //			}
 //			else
 //			{
 //				type[i] = ESmithFormatType::CENTER_NO_EFFECT;
 //			}
 //		}
 //		formatTest.SetupFormat(type, 9, 3, 3);
 //		SendAttackCommand(m_attackComp, formatTest, AttackHandle{GetName(), m_atk});

 //		--m_skillCnt;
 //		delete type;
 //		}
 //		else
 //		{
 //			// 移動の処理
 //			SendMoveCommand(m_moveComp,(UE::Smith::Battle::EMoveDirection)MoveDirection(),1);
 //		}

 //		m_timer = 0.0f;
 //	}
 //}

 //void ASmallGolem::OnSkill()
 //{
 //	MDebug::Log("Skill" + GetName());
 //	ASmithPlayerActor *player = Cast<ASmithPlayerActor>(target);
 //	player->OnAttack(AttackHandle{GetName(), m_atk + 2});
 //	m_skillCnt = m_skillCoolTurn;
 //}