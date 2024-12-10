// Fill out your copyright notice in the Description page of Project Settings.
/*

Smith

Author : MAI ZHICONG

Description : 3D Action Battle System (UE5.3.2)

Update History: 2024/11/18 Create

Version : alpha_1.0.0

*/

#pragma once

#ifndef SMITH_BATTLESYSTEM
#define SMITH_BATTLESYSTEM

#include "CoreMinimal.h"

class IAttack;
class IAttackable;

struct AttackParam
{
	int32 AttackValue;
	float MotionValue;
};

namespace Smith
{
	inline namespace Battle
	{
		class SmithBattleSystem;
	}
}

#define BATTLE_SYSTEM Smith::Battle::SmithBattleSystem
#define BATTLE_SYSTEM_PTR BATTLE_SYSTEM*

class ICanAccessBattleSystem
{
	public:
		ICanAccessBattleSystem(BATTLE_SYSTEM_PTR battleSys)
			: m_battleSystem(battleSys)
		{ }
		virtual ~ICanAccessBattleSystem() = 0 {};
	 
	protected:
		BATTLE_SYSTEM_PTR GetBattleSystem() const
		{
			return m_battleSystem.Get();
		}
	private:
		TSharedPtr<BATTLE_SYSTEM> m_battleSystem;
};

class IAttack
{
	public:
		IAttack()
		: m_param({})
		, m_attackerTag(TEXT("Default_Attacker"))
	{ }
		IAttack(const FString& attackerTag, int32 attackValue, float motionValue = 1.0f)
		: IAttack()
	{ 
		m_attackerTag = attackerTag;
		m_param.AttackValue = attackValue;
		m_param.MotionValue = motionValue;
	}

	public:
		void SetParam(int32 attackValue, float motionValue)
		{
			m_param.AttackValue = attackValue;
			m_param.MotionValue = motionValue;
		}
		void SetAttacker(const FString& attackerTag)
		{
			m_attackerTag = attackerTag;
		}

		AttackParam GetParam() const
		{
			return m_param;
		}

		bool IsSelfAttack(const FString& attackerTag)
		{
			return m_attackerTag.Equals(attackerTag);
		}

	public:
		virtual ~IAttack() = 0 {};

	private:
		AttackParam m_param;
		FString m_attackerTag;
};

namespace Smith
{
	inline namespace Battle
	{
		class SMITH_API SmithBattleSystem
		{
			public:
				SmithBattleSystem();
				~SmithBattleSystem();

			public:
				void RequestBattle(IAttack*, IAttackable*);

			private:
				int32 calculateDamage() const;

		};
	}
}

#undef BATTLE_SYSTEM
#undef BATTLE_SYSTEM_PTR

#endif