// Fill out your copyright notice in the Description page of Project Settings.
/*

TurnSystem Module

ITurnManageable.h

Author : MAI ZHICONG

Description : ターンシステムを利用するのに使うインターフェース

Update History: 2024/12/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/

#pragma once

#include "UObject/Interface.h"

#include "TurnPriority.h"

#include "ITurnManageable.generated.h"

UINTERFACE(MinimalAPI)
class UTurnManageable : public UInterface
{
	GENERATED_BODY()
};

/// 
/// @brief ターン管理ができるインターフェース
///
class TURNBATTLESYSTEM_API ITurnManageable
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
		///
		/// @brief コマンド送信許可設定
		///
		void SetCommandSendable(bool);
		///
		/// @brief ターン順番を設定
		///
		void SetTurnPriority(ETurnPriority);

		bool IsCommandSendable() const;
		ETurnPriority GetPriority() const;

	private:
		ETurnPriority m_priority = ETurnPriority::PlayerSelf;
		uint8 m_bIsCmdSendable : 1 = false;

	public:
		TMulticastDelegate<void()> OnTurnPass;
};
