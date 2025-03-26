// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_COMMAND_STRATEGY
#define SMITH_COMMAND_STRATEGY

#include "AttackHandle.h"
#include "Direction.h"

class AActor;
class USmithDamageSubsystem;

// Dummy
struct SmithDummyStrategy
{
	void operator()()
	{ }
};

struct SmithDefaultDamageStrategy
{
	void operator()();

	AActor* Instigator = nullptr;
	AActor* Causer = nullptr;
	USmithDamageSubsystem* DamageSubsystem = nullptr;
	FAttackHandle Handle = {};
	EDirection FromDirection;

};

struct SmithDefaultMoveStrategy
{
	void operator()(float DeltaTime);
	// TODO ストラテジー実行が完成できたかを確認する演算子オーバーロード
	explicit operator bool() const noexcept;

	AActor* MoveEntity = nullptr;
	float MoveSpeed = 0.f;
	FVector Destination;
};

#endif