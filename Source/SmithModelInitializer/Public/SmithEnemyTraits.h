// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_ENEMY_TRAITS
#define SMITH_ENEMY_TRAITS

#include <type_traits>

// 小ゴーレム
struct SmallGolem
{
	SMITHMODELINITIALIZER_API static const FString NAME;
};
// ドラゴン
struct Dragon
{
	SMITHMODELINITIALIZER_API static const FString NAME;
};
// 薬草ゴーレム
struct HerbGolem
{
	SMITHMODELINITIALIZER_API static const FString NAME;
};

template<typename EnemyType, bool bIsPtr> struct SmithEnemyTraits;

// ポインタTraits
template<typename EnemyType>
struct SmithEnemyTraits<EnemyType, true>
{
	using Type = typename std::remove_pointer_t<EnemyType>::Type;
};

// インスタンスTraits
template<typename EnemyType>
struct SmithEnemyTraits<EnemyType, false>
{
	using Type = typename std::decay_t<EnemyType>::Type;
};


#endif