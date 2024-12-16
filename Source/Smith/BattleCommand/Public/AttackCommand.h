// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

class ASmithPlayerActor;

class USmithMoveComponent;
/**
 * 
 */

namespace UE::Smith::Command
{
	class SMITH_API AttackCommand : public IBattleCommand
	{
	public:
		AttackCommand(USmithMoveComponent*);
		~AttackCommand();

	public:
		virtual void Start() override;
		virtual void Execute(float deltaTime) override;
		virtual void End() override;
		virtual bool IsFinish() const override;

	private:
		TWeakObjectPtr<USmithMoveComponent> m_moveComp;
	};
}
