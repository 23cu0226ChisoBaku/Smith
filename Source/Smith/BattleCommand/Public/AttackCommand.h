// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

class ASmithPlayerActor;
/**
 * 
 */
class SMITH_API AttackCommand : public IBattleCommand
{
public:
	AttackCommand(ASmithPlayerActor*);
	~AttackCommand();

public:
	void Execute() override final;

private:
	TWeakObjectPtr<ASmithPlayerActor> m_actor;
};
