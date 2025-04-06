// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"

#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "SmithEnemyGenerateBluePrint.h"
#include "Misc/DateTime.h"
#include "SmithMapDecoration.h"

#include "SmithDungeonBattleGameMode.generated.h"

class USmithCommandMediator;
class USmithChasePlayerTracker;
class USmithBattleSubsystem;
class USmithEventPublisher;
class USmithEventSystem;
class USmithEnhanceSubsystem;
class USmithEventPublishMediator;
class USmithBattleLogWorldSubsystem;
class UUI_CurrentLevel;
class USmithNextLevelEvent;
class USmithMinimap;
class UMinimapDisplayTypeFactory;
class UGameLogWidget;
class UScreenFade;

class UMapModelMapper;
class UEventModelMapper;
class UBattleLogModelMapper;

namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapSystem;
	}
}

/**
 * ダンジョンゲームモード
 */
UCLASS()
class SMITHGAMEPLAY_API ASmithDungeonBattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
//---------------------------------------
/*
                  ctor
*/
//---------------------------------------
public:
	ASmithDungeonBattleGameMode();

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
	void processGameOver();
	void showMinimap();
	void hideMinimap();

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
	TObjectPtr<USmithCommandMediator> m_battleMediator;

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

	UPROPERTY(EditAnywhere, meta=(RequiredAssetDataTags = "RowStructure=/Script/SmithModel.SmithPickableDropRateListRow"))
	TObjectPtr<UDataTable> EnemyDropLootList;

	UPROPERTY(EditAnywhere)
	FSmithMapDecoration TEMP_Decoration;


	UPROPERTY(EditAnywhere, Category = "Minimap")
	TSubclassOf<USmithMinimap> MinimapWidgetSub;
	UPROPERTY()
	TObjectPtr<USmithMinimap> m_minimap;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	TSubclassOf<UMinimapDisplayTypeFactory> MinimapTypeFactorySub;
	UPROPERTY()
	TObjectPtr<UMinimapDisplayTypeFactory> m_factory;

	UPROPERTY(EditDefaultsOnly, Category = "ModelMapper", meta = (MustImplement = "MapModelGateway"))
	TObjectPtr<UMapModelMapper> MapModelMapper;

	UPROPERTY(EditDefaultsOnly, Category = "ModelMapper", meta = (MustImplement = "BattleLogModelGateway"))
	TObjectPtr<UBattleLogModelMapper> LogModelMapper;

	UPROPERTY(EditDefaultsOnly, Category = "ModelMapper", meta = (MustImplement = "EventModelGateway"))
	TObjectPtr<UEventModelMapper> EventModelMapper;

private:
	TSharedPtr<UE::Smith::Map::FSmithMapSystem> m_mapSys;
	// 倒された敵の数
	int32 m_defeatedEnemyCount;
	int32 m_curtLevel;
	int32 m_startPlayTime;

// TODO Test
private:
	FDateTime m_startDateTime;

};
