// Fill out your copyright notice in the Description page of Project Settings.
/*

Weapon_Base.h

Author:KETNO UENO

UpdateHistroy:
2024,12,23 �J�n

Version:

Encoding: UTF-8

*/


#pragma once

#include "CoreMinimal.h"

struct WeaponStatus
{
	FString Name;	// ���O
	uint8 ATK;			// �U����
	uint8 DEF;			// �h���
	float CRT;			// �N���e�B�J����
};

struct SkillBase
{
	FString Name;	// ���O
	uint8 BasicValue;	// ��b�l
	// ���ʔ͈�
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
