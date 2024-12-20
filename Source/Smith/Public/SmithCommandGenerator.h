// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class IBattleCommand;
class IMoveable;

class ICanMakeAttack;
class IAttackable;

struct AttackHandle;

/**
 * 
 */
class SMITH_API SmithCommandGenerator
{
public:
	static TSharedPtr<IBattleCommand> MakeMoveCommand(IMoveable* moveRequester);
	static TSharedPtr<IBattleCommand> MakeAttackCommand(ICanMakeAttack* attackRequester, IAttackable* target, AttackHandle&& handle); 
};
