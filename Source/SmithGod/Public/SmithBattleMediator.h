// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "ICommandMediator.h"

#include "UObject/WeakInterfacePtr.h"

#include "SmithBattleMediator.generated.h"

class USmithBattleSubsystem;
class USmithDamageSubsystem;

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
	void SetupMediator(USmithBattleSubsystem*, USmithDamageSubsystem*, TSharedPtr<MapManager>);

public:
	bool SendMoveCommand(AActor*, IMoveable*, EDirection, uint8 moveDistance) override final;
	bool SendAttackCommand(AActor*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&, bool bAttackEvenNoTarget = true) override final;
	bool SendSkillCommand(AActor*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&) override final;
	bool SendIdleCommand(AActor*, float idleTime) override final;

public:
	int32 GetRangeLocations(TArray<FVector>& outLocations, AActor*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&) override final;
	void GetPlayerDirection(EDirection& outDirection, EDirectionStrategy, AActor*, uint8 offsetToLeft, uint8 offsetToTop) override final;

private:

	UPROPERTY()
	TObjectPtr<USmithBattleSubsystem> m_battleSys;

	UPROPERTY()
	TObjectPtr<USmithDamageSubsystem> m_damageSys;

private:

	TWeakPtr<MapManager> m_mapMgr;
};
