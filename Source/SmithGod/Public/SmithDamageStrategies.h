// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_DAMAGE_STRATEGY
#define SMITH_DAMAGE_STRATEGY

#include "AttackHandle.h"

class AActor;
class USmithDamageSubsystem;

// Strategy Dummy
struct SMITHGOD_API SmithEmptyDamageStrategy
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
	AttackHandle Handle;

};

#endif