// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SmithEnhanceableEQInstance.h"

#include "SmithWeapon_Sword.generated.h"

UCLASS()
class SMITH_API USmithWeapon_Sword : public USmithEnhanceableEQInstance
{
	GENERATED_BODY()

private:

	//-----------------------------------------
	// USmithEquipmentInstance method override
	//-----------------------------------------

	void onEquippedImpl() override;
	void onUnequippedImpl() override;

	//---------------------------------------------
	// USmithEnhanceableEQInstance method override
	//---------------------------------------------
	
	void onEnhancedImpl() override;
};
