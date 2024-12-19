// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithCommandGenerator.h"

#include "MoveCommand.h"
#include "IMoveable.h"

#include "AttackCommand.h"
#include "ICanMakeAttack.h"
#include "IAttackable.h"
#include "AttackHandle.h"

TSharedPtr<IBattleCommand> SmithCommandGenerator::MakeMoveCommand(IMoveable* moveRequester)
{
  return ::MakeShared<UE::Smith::Command::MoveCommand>(moveRequester);
}

TSharedPtr<IBattleCommand> SmithCommandGenerator::MakeAttackCommand(ICanMakeAttack* attackRequester, IAttackable* target, AttackHandle&& handle)
{
  return ::MakeShared<UE::Smith::Command::AttackCommand>(attackRequester, target, ::MoveTemp(handle));
}
