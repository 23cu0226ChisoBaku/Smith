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
	class SMITH_API MoveCommand : public IBattleCommand
	{
	public:
		MoveCommand(USmithMoveComponent*);
		~MoveCommand();

	public:
		virtual void Start() override;
		virtual void Execute(float deltaTime) override;
		virtual void End() override;

	private:
		TWeakObjectPtr<USmithMoveComponent> m_moveComp;
	};
}
