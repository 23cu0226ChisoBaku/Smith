// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef ATTACKABLE_INFO_HANDLE
#define ATTACKABLE_INFO_HANDLE

#include "Direction.h"

class AActor;

struct MAPMANAGEMENT_API FAttackableInfoHandle
{
	AActor* Attackable;
	EDirection AttackFrom;

	constexpr FAttackableInfoHandle()
		: Attackable(nullptr)
		, AttackFrom(EDirection::Invalid)
	{}

	FAttackableInfoHandle(AActor* Attackable_, EDirection From)
		: Attackable(Attackable_)
		, AttackFrom(From)
	{}

};

#endif
