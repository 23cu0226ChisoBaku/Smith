// Fill out your copyright notice in the Description page of Project Settings.
/*

SpawnActorGameMode_Test.h

Author : MAI ZHICONG

Description : Test Game Mode(Spawn Actor)

Update History: 2024/12/24 作成

Version : alpha_1.0.0

Encoding : UTF-8 
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnActorGameMode_Test.generated.h"

//---------------------------------------
/*
                  前方宣言
*/
//---------------------------------------
class USmithBattleMediator;
namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapManager;
	}
}

///
/// @brief ダンジョン用テスト
///
UCLASS()
class SMITHGOD_API ASpawnActorGameMode_Test : public AGameModeBase
{
	GENERATED_BODY()

//---------------------------------------
/*
                  ctor
*/
//---------------------------------------
public:
	ASpawnActorGameMode_Test();

//---------------------------------------
/*
           アクター ライフサイクル
*/
//---------------------------------------
public:
	void StartPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

//---------------------------------------
/*
              uproperty 宣言
*/
//---------------------------------------
private:
	UPROPERTY()
	TObjectPtr<USmithBattleMediator> m_battleMediator;
	
//---------------------------------------
/*
            プライベートプロパティ
*/
//---------------------------------------
private:
	TSharedPtr<UE::Smith::Map::FSmithMapManager> m_mapMgr;
};
