// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/// @brief  バトルログタイプ
/// リッチテキスト変換に使う
UENUM()
enum class EBattleLogModelType : uint8
{
	Player,
	Enemy,
	Item,
	Enhance,
	TypeCount UMETA(Hidden),
};