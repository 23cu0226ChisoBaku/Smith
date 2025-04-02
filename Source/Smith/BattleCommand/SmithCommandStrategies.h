// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_COMMAND_STRATEGY
#define SMITH_COMMAND_STRATEGY

#include "AttackHandle.h"
#include "Direction.h"

class AActor;
class ISmithDamageApplicant;

// Dummy
struct SMITH_API SmithDummyStrategy
{
	// ストラテジーを実行する
	void operator()()
	{ }
};

struct SMITH_API SmithDefaultDamageStrategy
{
	// ストラテジーを実行する
	void operator()();

	AActor* Instigator = nullptr;												// 攻撃する者
	AActor* Causer = nullptr;														// 攻撃をくらう者
	ISmithDamageApplicant* DamageApplicant = nullptr;		// ダメージ処理者インターフェース
	FAttackHandle Handle = {};													// 攻撃ハンドル
	EDirection FromDirection;														// 攻撃がくる方向

};

struct SMITH_API SmithDefaultMoveStrategy
{
	// ストラテジーを実行する
	void operator()(float DeltaTime);
	// TODO ストラテジー実行が完成できたかを確認する演算子オーバーロード
	explicit operator bool() const noexcept;

	AActor* MoveEntity = nullptr;					// 移動するオブジェクト
	float MoveSpeed = 0.f;								// 移動速度
	FVector Destination;									// 移動先の座標
};

#endif