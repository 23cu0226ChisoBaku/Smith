// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_DAMAGE_STRATEGY
#define SMITH_DAMAGE_STRATEGY

#include "AttackHandle.h"
#include "Direction.h"

class AActor;
class USmithDamageSubsystem;

// Dummy
struct SMITHGOD_API SmithDummyStrategy
{
	void operator()()
	{ }
};

struct SMITHGOD_API SmithDefaultDamageStrategy
{
	void operator()();

	AActor* Instigator = nullptr;
	AActor* Causer = nullptr;
	USmithDamageSubsystem* DamageSubsystem = nullptr;
	FAttackHandle Handle;
	EDirection FromDirection;

};

#endif