// Fill out your copyright notice in the Description page of Project Settings.
/*

Weapon_Base.h

Author:KETNO UENO

UpdateHistroy:
2024,12,23 ŠJŽn

Version:

Encoding: UTF-8

*/


#pragma once

#include "SmithActors/Weapon/Params.h"
#include "SmithActors/Weapon/Skill.h"
#include "Smith/Public/ParamAbsorbable.h"
#include "CoreMinimal.h"



class SMITH_API Weapon_Base
{
public:
	Weapon_Base();
	virtual ~Weapon_Base();

	void SwapSkill(int, FSkill);
	void Upgrade(IParamAbsorbable*);
};
