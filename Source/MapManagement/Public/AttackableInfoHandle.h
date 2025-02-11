// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef ATTACKABLE_INFO_HANDLE
#define ATTACKABLE_INFO_HANDLE

#include "CoreMinimal.h"
#include "IAttackable.h"
#include "Direction.h"

struct MAPMANAGEMENT_API FAttackableInfoHandle
{

	IAttackable* Attackable;
	EDirection AttackFrom;

	constexpr FAttackableInfoHandle()
		: Attackable(nullptr)
		, AttackFrom(EDirection::Invalid)
	{}

	FAttackableInfoHandle(IAttackable* attackable, EDirection attackFrom)
		: Attackable(attackable)
		, AttackFrom(attackFrom)
	{}

};

#endif
