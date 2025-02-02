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
class USmithBattleSubsystem;
class USmithEventPublisher;
class USmithEventSystem;
class USmithEnhanceSubsystem;
class USmithEventPublishMediator;
class USmithBattleLogWorldSubsystem;
class UUI_CurrentLevel;

// Damage
class USmithDungeonDamageCalculator;

class UGameLogWidget;

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
	void initializeGame();
	void startNewLevel();
	void clearCurrentLevel();

// TODO
public:
	void goToNextLevel();

private:
	/** ダンジョンマップ設計図 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithMapBluePrint MapBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithMapConstructionBluePrint MapConstructionBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithEnemyGenerateBluePrint EnemyGenerateBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithMapBluePrint BossMapBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true"))
	FSmithEnemyGenerateBluePrint BossGenerateBluePrint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameLogWidget> LogWidgetSub;

	UPROPERTY()
	TObjectPtr<USmithBattleSubsystem> m_battleSystem;
	UPROPERTY()
	TObjectPtr<USmithBattleMediator> m_battleMediator;
	UPROPERTY()
	TObjectPtr<USmithEventPublisher> m_eventPublisher;
	UPROPERTY()
	TObjectPtr<USmithEventSystem> m_eventSystem;
	UPROPERTY()
	TObjectPtr<USmithChasePlayerTracker> m_chasePlayerTracker;
	UPROPERTY()
	TObjectPtr<USmithEnhanceSubsystem> m_enhanceSystem;
	UPROPERTY()
	TObjectPtr<USmithEventPublishMediator> m_eventMediator;
	UPROPERTY()
	TObjectPtr<USmithBattleLogWorldSubsystem> m_logSubsystem;
	UPROPERTY()
	TObjectPtr<USmithDungeonDamageCalculator> m_damageCalculator;

	UPROPERTY(EditAnywhere, Category = TEST)
	TSubclassOf<AActor> TEST_ACTOR;

	UPROPERTY(EditAnywhere)
	double TEST_DAMAGE_CALCULATOR_CONSTANT;

	UPROPERTY()
	TObjectPtr<UUI_CurrentLevel> CurtLevelUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUI_CurrentLevel> LevelUISub;

private:
	TSharedPtr<UE::Smith::Map::FSmithMapManager> m_mapMgr;
	uint8 m_curtLevel;
};
