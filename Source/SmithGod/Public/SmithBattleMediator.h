// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ICommandMediator.h"
#include "SmithBattleMediator.generated.h"

class USmithBattleSubsystem;
namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapManager;
	}
}
/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithBattleMediator final: public UObject, public ICommandMediator
{
	GENERATED_BODY()

public:
	using MapManager = typename UE::Smith::Map::FSmithMapManager;

public:
	USmithBattleMediator();

public:
	void BeginDestroy() override final;
	void SetupMediator(USmithBattleSubsystem*, TSharedPtr<MapManager>);

public:
	void SendMoveCommand(AActor*, IMoveable*, UE::Smith::Battle::EMoveDirection, uint8 moveDistance) override final;
	void SendAttackCommand(AActor*, ICanMakeAttack*, UE::Smith::Battle::EMoveDirection, const UE::Smith::Battle::FSmithCommandFormat&, AttackHandle&&) override final;

private:
	UPROPERTY()
	TWeakObjectPtr<USmithBattleSubsystem> m_battleSys;

private:
	TWeakPtr<MapManager> m_mapMgr;
};
