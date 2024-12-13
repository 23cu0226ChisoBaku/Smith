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
		void Execute() override final;

	private:
		//TWeakObjectPtr<USmithMoveComponent> m_moveComp;
	};
}
