// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

class IMoveable;
class ISmithAnimator;

namespace UE::Smith::Command
{
	class SMITH_API MoveCommand final : public IBattleCommand
	{
	public:
		MoveCommand(IMoveable*, ISmithAnimator* = nullptr);
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
