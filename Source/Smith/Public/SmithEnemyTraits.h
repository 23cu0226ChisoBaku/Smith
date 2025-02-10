// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_ENEMY_TRAITS
#define SMITH_ENEMY_TRAITS

#include <type_traits>

// 小ゴーレム
struct SmallGolem{};
// ドラゴン
struct Dragon{};
// 薬草ゴーレム
struct HerbGolem{};

template<typename EnemyType, bool bIsPtr> struct SmithEnemyTraits;

// ポインタだったら
template<typename EnemyType>
struct SmithEnemyTraits<EnemyType, true>
{
	using Type = typename std::remove_pointer_t<EnemyType>::Type;
};

// インスタントだったら
template<typename EnemyType>
struct SmithEnemyTraits<EnemyType, false>
{
	using Type = typename EnemyType::Type;
};


#endif