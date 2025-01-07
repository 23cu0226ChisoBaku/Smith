// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SmithMapBluePrint.h"
#include "SmithMapConstructionBluePrint.h"
#include "MapGenerateGameMode_Test.generated.h"

class USmithBattleMediator;
namespace UE::Smith
{
	namespace Map
	{
		class FSmithMap;
		class FSmithMapManager;
	}
}

/**
 * 
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FSmithMapBluePrint MapBluePrint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FSmithMapConstructionBluePrint MapConstructionBluePrint;
	UPROPERTY()
	TObjectPtr<USmithBattleMediator> m_battleMediator;

private:
	TSharedPtr<UE::Smith::Map::FSmithMapManager> m_mapMgr;
};
