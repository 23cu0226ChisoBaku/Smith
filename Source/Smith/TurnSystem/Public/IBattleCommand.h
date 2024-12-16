// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

/// @brief バトルコマンド
class SMITH_API IBattleCommand 
{
public:
	// コマンドを執行する
	virtual void Start() = 0;
	virtual void Execute(float deltaTime) = 0;
	virtual void End() = 0;
	virtual bool IsFinish() const = 0;

	virtual ~IBattleCommand() = 0 {};
};
