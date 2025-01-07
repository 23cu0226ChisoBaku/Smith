// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

class ISkillable;

namespace UE::Smith::Command
{
  class SMITH_API SkillCommand : public IBattleCommand
  {
  public:
    SkillCommand(ISkillable*);
    ~SkillCommand();

  public:
    void Start() override final;
    void Execute(float deltaTime) override final;
    void End() override final;
    bool IsFinish() const override final;

  private:
    class SkillImpl;
    TUniquePtr<SkillImpl> m_skillImpl;
  };
}