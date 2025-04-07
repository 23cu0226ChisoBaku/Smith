// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmithEquipmentInstance.h"

#include "IEnhanceable.h"

#include "SmithEnhanceableEQInstance.generated.h"

UCLASS(Abstract)
class SMITH_API USmithEnhanceableEQInstance : public USmithEquipmentInstance , public IEnhanceable
{
	GENERATED_BODY()

public:

	//---------------------------------------------
	// IEnhanceable Interface implementation(実装)
	//---------------------------------------------
	// FIXME 強化（削除する予定）
	void Enhance(IParamAbsorbable*) override;

	/// @brief 強化された時に呼び出されるコールバック
	void OnEnhanced() override;

private:

	/// @brief 強化された時の実装
	virtual void onEnhancedImpl() PURE_VIRTUAL(ThisClass::onEnhancedImpl,);
	
};
