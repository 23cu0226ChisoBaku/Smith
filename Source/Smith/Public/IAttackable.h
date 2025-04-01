// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "BattleResult.h"

#include "IAttackable.generated.h"

struct FBattleDefenseParamHandle;

UINTERFACE(MinimalAPI)
class UAttackable : public UInterface
{
	GENERATED_BODY()
};

class SMITH_API IAttackable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	///
	/// @brief          攻撃を受けたとき呼び出されるコールバック
	/// @param Result   くらった攻撃の結果
	///
	virtual void OnAttack(const FBattleResult& Result) = 0;
	///
	///	@brief          倒されたかを調べる
	/// @return         true->倒された false->倒していない
	///
	virtual bool IsDefeated() const = 0;
  ///
  /// @brief          倒された時に呼び出されるコールバック
  ///
	virtual void OnDefeated() = 0;
  ///
  /// @brief          防衛パラメーターハンドル取得（TODO：修正する予定）
  /// @return         
  ///
	virtual FBattleDefenseParamHandle GetDefenseParam() const;
};
