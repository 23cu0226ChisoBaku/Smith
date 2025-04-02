// Fill out your copyright notice in the Description page of Project Settings.
/*

TurnPriority.h

Author : MAI ZHICONG

Description : バトル優先順位列挙型

Update History: 2024/12/08 作成

Version : alpha_1.0.0

Encoding : UTF-8 

*/
#pragma once

UENUM(BlueprintType)
enum class ETurnPriority : uint8
{
	PlayerSelf = 0,			// プレイヤー
	Rival = 1,					// 敵
	Alliance = 2,				// 仲間

	PriorityTypeCnt UMETA(Hidden),		// ターンの種類をカウントするため、使わないでください
};

