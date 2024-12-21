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
#include "ITurnManageable.generated.h"

class UTurnControlComponent;
class IBattleCommand;
class ITurnManageable;

///
/// @brief コマンドを出す専用デリゲート
///
DECLARE_MULTICAST_DELEGATE_TwoParams(FRequestCommandEvent, ITurnManageable*, TSharedPtr<IBattleCommand>);


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
	/// TODO 後で直す
	/// @brief ターンコントローラーを取得
	virtual UTurnControlComponent* GetTurnControl() const = 0;
	/// @brief コマンド発送イベントを購読
	virtual FDelegateHandle Subscribe(FRequestCommandEvent::FDelegate&) = 0;
	/// @brief コマンド発送イベントを解読
	virtual bool Unsubscribe(UObject*,FDelegateHandle) = 0;
};
