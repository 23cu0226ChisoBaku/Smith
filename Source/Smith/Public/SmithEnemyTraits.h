// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef SMITH_ENEMY_TRAITS
#define SMITH_ENEMY_TRAITS

// 小ゴーレム
struct SmallGolem{};
// ドラゴン
struct Dragon{};

template<typename EnemyType> 
struct SmithEnemyTraits
{
	using Type = EnemyType::Type;
};

#endif