// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "ICommandMediator.h"

#include "UObject/WeakInterfacePtr.h"

#include "SmithCommandMediator.generated.h"

class USmithBattleSubsystem;
class USmithDamageSubsystem;

namespace UE::Smith
{
	namespace Map
	{
		class FSmithMapSystem;
	}
}
/**
 * 
 */
UCLASS()
class SMITHGAMEPLAY_API USmithCommandMediator final: public UObject, public ICommandMediator
{
	GENERATED_BODY()

public:
	using MapSystem = typename UE::Smith::Map::FSmithMapSystem;

public:
	USmithCommandMediator();

public:
	void BeginDestroy() override final;
	void SetupMediator(USmithBattleSubsystem*, USmithDamageSubsystem*, TSharedPtr<MapSystem>);

//---Begin of ICommandMediator Interface
public:
	bool SendMoveCommand(AActor*, EDirection, uint8 moveDistance) override final;
	bool SendAttackCommand(AActor*, EDirection, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&, bool bAttackEvenNoTarget = true) override final;
	bool SendSkillCommand(AActor*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&, const FAttackHandle&) override final;
	bool SendIdleCommand(AActor*, float idleTime) override final;
	int32 GetRangeLocations(TArray<FVector>& outLocations, AActor*, FSmithSkillParameter, const UE::Smith::Battle::FSmithCommandFormat&) override final;
	void GetPlayerDirection(EDirection& outDirection, EDirectionPolicy, AActor*, uint8 offsetToLeft, uint8 offsetToTop) override final;

//---End of ICommandMediator Interface
private:

	UPROPERTY()
	TObjectPtr<USmithBattleSubsystem> m_battleSys;

	UPROPERTY()
	TObjectPtr<USmithDamageSubsystem> m_damageSys;

private:

	TWeakPtr<MapSystem> m_mapSys;
};
