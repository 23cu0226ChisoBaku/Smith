// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SmithBattleMediator.generated.h"

class USmithBattleSubsystem;
namespace Smith::Map
{
	class SmithMapManager;
}
/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithBattleMediator final: public UObject
{
	GENERATED_BODY()

public:
	using MapManager = typename Smith::Map::SmithMapManager;

public:
	USmithBattleMediator();

public:
	void BeginDestroy() override final;
	void SetupMediator(USmithBattleSubsystem*, TSharedPtr<MapManager>);

private:
	UPROPERTY()
	TWeakObjectPtr<USmithBattleSubsystem> m_battleSys;

private:
	TWeakPtr<MapManager> m_mapMgr;
	
};
