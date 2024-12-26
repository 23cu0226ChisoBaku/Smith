// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

class IMoveable;

namespace UE::Smith::Command
{
	class SMITH_API MoveCommand : public IBattleCommand
	{
	public:
		MoveCommand(IMoveable*);
		~MoveCommand();

	public:
		void Start() override final;
		void Execute(float deltaTime) override final;
		void End() override final;
		bool IsFinish() const override final;

	private:
		class MoveImpl;
		TUniquePtr<MoveImpl> m_moveImpl;
	};
}
