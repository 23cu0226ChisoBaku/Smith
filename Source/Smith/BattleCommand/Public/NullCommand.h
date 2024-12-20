// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

/**
 * 
 */

namespace UE::Smith::Command
{
	class SMITH_API NullCommand final: public IBattleCommand
	{
	public:
		NullCommand();
		~NullCommand();

	public:
		void Start() override final;
		void Execute(float deltaTime) override final;
		void End() override final;
		bool IsFinish() const override final;
	};
}
