// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "MapGenerateGameMode_Test.generated.h"

class USmithBattleMediator;
class USmithChasePlayerTracker;
namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapManager;
	}
}

/**
 * ダンジョンゲームモード
 */
UCLASS()
class SMITHGOD_API AMapGenerateGameMode_Test : public AGameModeBase
{
	GENERATED_BODY()
//---------------------------------------
/*
                  ctor
*/
//---------------------------------------
public:
	AMapGenerateGameMode_Test();

public:
	void StartPlay() override final;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

private:
	void StartNewStage();

private:
	/** ダンジョンマップ設計図 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithMapBluePrint MapBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithMapConstructionBluePrint MapConstructionBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithEnemyGenerateBluePrint EnemyGenerateBluePrint;
	UPROPERTY()
	TObjectPtr<USmithBattleMediator> m_battleMediator;
	UPROPERTY()
	TObjectPtr<USmithChasePlayerTracker> m_chasePlayerTracker;

private:
	TSharedPtr<UE::Smith::Map::FSmithMapManager> m_mapMgr;
};
