  // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_BATTLE_COMMAND_FACTORY
#define SMITH_BATTLE_COMMAND_FACTORY

#include "SmithBattleCommand.h"
// TODO
#include "ISmithAnimator.h"

#include "AttackCommand.h"
#include "MoveCommand.h"
#include "SkillCommand.h"
#include "IdleCommand.h"

namespace SmithCmd = UE::Smith::Command;

class FBattleCommandFactory
{

  private:

    FBattleCommandFactory() {}
    ~FBattleCommandFactory() {}

  public:

    static TSharedPtr<IBattleCommand> CreateIdleCommand(float IdleTime = 0.0f)
    {
      return ::MakeShared<FSmithBattleCommand>(SmithCmd::IdleCommand(IdleTime));
    }

    template<ConceptMoveStrategy MoveStrategy>
    static TSharedPtr<IBattleCommand> CreateMoveCommand(MoveStrategy Strategy_, ISmithAnimator* Animator = nullptr)
    {
      return ::MakeShared<FSmithBattleCommand>(SmithCmd::MoveCommand(::MoveTemp(Strategy_), Animator));
    }

    template<ConceptAttackStrategy AttackStrategy>
    static TSharedPtr<IBattleCommand> CreateAttackCommand(AttackStrategy Strategy_, ISmithAnimator* Animator = nullptr)
    {
      return ::MakeShared<FSmithBattleCommand>(SmithCmd::AttackCommand(::MoveTemp(Strategy_), Animator));
    }

    template<ConceptAttackStrategy AttackStrategy>
    static TSharedPtr<IBattleCommand> CreateSkillCommand(AttackStrategy Strategy_, ISmithAnimator* Animator, uint8 SkillSlot)
    {
      return ::MakeShared<FSmithBattleCommand>(SmithCmd::SkillCommand(::MoveTemp(Strategy_), Animator, SkillSlot));
    }
};

#endif
