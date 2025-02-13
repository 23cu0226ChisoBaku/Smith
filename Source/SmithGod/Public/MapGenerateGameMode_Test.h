// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "Misc/DateTime.h"
#include "SmithMapDecoration.h"
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
class USmithNextLevelEvent;
class USmithTowerEnemyParamInitializer;

// Damage
class USmithDungeonDamageCalculator;
class UGameLogWidget;
class UScreenFade;

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

public:
	UFUNCTION(BlueprintCallable)
	int32 GetDefeatedEnemyCount() const;
	UFUNCTION(BlueprintCallable)
	int32 GetUpgradeCount() const;
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentLevel() const;
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentPlayTime_Second() const;
	UFUNCTION(BlueprintCallable)
	FTimespan GetCurrentPlayTime_Timespan() const;

private:
	void initializeGame();
	void startNewLevel();
	void clearCurrentLevel();
	void deployNextLevelEvent(bool bIsActiveWhenDeploy = true);
	void deployPickableEvent();
	void goToNextLevel();
	void addDefeatedEnemyCount();
	void processGameClear();

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = MapGeneration, meta = (AllowPrivateAccess = "true",RequiredAssetDataTags = "RowStructure=/Script/SmithModel.ItemGenerationListRow"))
	TObjectPtr<UDataTable> ItemGenerationRecipe;
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
	UPROPERTY(EditAnywhere)
	double TEST_DAMAGE_CALCULATOR_CONSTANT;
	UPROPERTY()
	TObjectPtr<UUI_CurrentLevel> CurtLevelUI;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUI_CurrentLevel> LevelUISub;
	UPROPERTY()
	TObjectPtr<UScreenFade> m_fadeWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenFade> FadeSub;
	UPROPERTY()
	TObjectPtr<USmithNextLevelEvent> m_nextLevelEvent;
	UPROPERTY(EditAnywhere, meta=(RequiredAssetDataTags = "RowStructure=/Script/SmithModel.SmithEnemyParamInitializerRow"))
	TObjectPtr<UDataTable> EnemyDefaultParamList;
	UPROPERTY(EditAnywhere, meta=(RequiredAssetDataTags = "RowStructure=/Script/Smith.SmithPickableDropRateListRow"))
	TObjectPtr<UDataTable> EnemyDropLootList;
	UPROPERTY()
	TObjectPtr<USmithTowerEnemyParamInitializer> m_towerInitializer;

	UPROPERTY(EditAnywhere)
	FSmithMapDecoration TEMP_Decoration;

private:
	TSharedPtr<UE::Smith::Map::FSmithMapManager> m_mapMgr;
	// 倒された敵の数
	int32 m_defeatedEnemyCount;
	int32 m_curtLevel;
	int32 m_startPlayTime;

// TODO Test
private:
	FDateTime m_startDateTime;

};
