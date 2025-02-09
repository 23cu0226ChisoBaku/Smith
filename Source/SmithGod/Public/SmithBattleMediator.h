// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ICommandMediator.h"
#include "UObject/WeakInterfacePtr.h"
#include "SmithBattleMediator.generated.h"

class USmithBattleSubsystem;
class ISmithDamageCalculator;
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
	void SetupMediator(USmithBattleSubsystem*, ISmithDamageCalculator*, TSharedPtr<MapManager>);

public:
	bool SendMoveCommand(AActor*, IMoveable*, EDirection, uint8 moveDistance) override final;
	bool SendAttackCommand(AActor*, ICanMakeAttack*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, AttackHandle&&, bool bAttackEvenNoTarget) override final;
	bool SendAttackCommand(AActor*, ICanMakeAttack*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&, bool bAttackEvenNoTarget = true) override final;
	bool SendSkillCommand(AActor*, ICanMakeAttack*, FSmithSkillCenterSpotParameter, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&) override final;
	bool SendIdleCommand(AActor*) override final;

public:
	int32 GetRangeLocations(TArray<FVector>& outLocations, AActor*, FSmithSkillCenterSpotParameter, const UE::Smith::Battle::FSmithCommandFormat&) const override final;
private:
	UPROPERTY()
	TWeakObjectPtr<USmithBattleSubsystem> m_battleSys;

	TWeakInterfacePtr<ISmithDamageCalculator> m_damageCalculator;

private:
	TWeakPtr<MapManager> m_mapMgr;
};
