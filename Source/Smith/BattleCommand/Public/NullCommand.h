// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IBattleCommand.h"

/**
 * 
 */
class SMITH_API NullCommand : public IBattleCommand
{
public:
	NullCommand();
	~NullCommand();

public:
	void Execute() override;
};
