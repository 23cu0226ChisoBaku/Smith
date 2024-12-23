// Fill out your copyright notice in the Description page of Project Settings.
/*

Weapon_Base.h

Author:KETNO UENO

UpdateHistroy:
2024,12,23 開始

Version:

Encoding: UTF-8

*/


#pragma once

#include "CoreMinimal.h"

struct WeaponStatus
{
	FString Name;	// 名前
	uint8 ATK;			// 攻撃力
	uint8 DEF;			// 防御力
	float CRT;			// クリティカル率
};

struct SkillBase
{
	FString Name;	// 名前
	uint8 BasicValue;	// 基礎値
	// 効果範囲
};

class SMITH_API Weapon_Base
{


public:
	Weapon_Base();
	virtual ~Weapon_Base();

private:
	WeaponStatus GetWeaponStatus() {};

private:
};
