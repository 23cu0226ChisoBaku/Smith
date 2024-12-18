// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

class USmithAttackComponent;
class IAttackable;
struct AttackHandle;
/**
 * 
 */

namespace UE::Smith::Command
{
	class SMITH_API AttackCommand : public IBattleCommand
	{
	public:
		AttackCommand(USmithAttackComponent*, IAttackable*, AttackHandle&&);
		~AttackCommand();

	public:
		virtual void Start() override;
		virtual void Execute(float deltaTime) override;
		virtual void End() override;
		virtual bool IsFinish() const override;

	private:
		TWeakObjectPtr<USmithAttackComponent> m_atkComp;
	};
}
