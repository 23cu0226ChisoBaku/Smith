// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "IBattleCommand.h"

class ASmithPlayerActor;
class IMoveable;
/**
 * 
 */

namespace UE::Smith::Command
{
	class SMITH_API MoveCommand : public IBattleCommand
	{
	public:
		MoveCommand(IMoveable*);
		~MoveCommand();

	public:
		virtual void Start() override;
		virtual void Execute(float deltaTime) override;
		virtual void End() override;
		virtual bool IsFinish() const override;

	private:
		TWeakInterfacePtr<IMoveable> m_move;
	};
}
