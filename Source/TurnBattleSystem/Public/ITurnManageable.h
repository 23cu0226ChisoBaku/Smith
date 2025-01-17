// Fill out your copyright notice in the Description page of Project Settings.
/*

TurnSystem Module

ITurnManageable.h

Author : MAI ZHICONG

Description : ターンシステムを利用するのに使うインターフェース

Update History: 2024/12/11 Create

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TurnPriority.h"
#include "ITurnManageable.generated.h"

class IBattleCommand;
class ITurnManageable;

UINTERFACE(MinimalAPI)
class UTurnManageable : public UInterface
{
	GENERATED_BODY()
};

/// 
/// @brief ターン管理ができる
///
class TURNBATTLESYSTEM_API ITurnManageable
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
		void SetCommandSendable(bool);
		void SetTurnPriority(ETurnPriority);

		bool IsCommandSendable() const;
		ETurnPriority GetPriority() const;

	private:
		ETurnPriority m_priority = ETurnPriority::PlayerSelf;
		uint8 m_bIsCmdSendable : 1 = false;
};
