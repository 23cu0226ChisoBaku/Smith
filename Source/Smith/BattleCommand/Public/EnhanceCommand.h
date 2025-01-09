// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

namespace UE::Smith::Command
{
	class SMITH_API EnhanceCommand final: public IBattleCommand
	{
	public:
		EnhanceCommand();
		~EnhanceCommand();

	public:
		void Start() override final;
		void Execute(float deltaTime) override final;
		void End() override final;
		bool IsFinish() const override final;

	private:
		class EnhanceImpl;
		TUniquePtr<EnhanceImpl> m_enhanceImpl;
	};
}