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
	virtual void Execute() = 0;

	virtual ~IBattleCommand() = 0 {};
};
