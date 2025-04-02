// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "IEnhanceSystem.h"
#include "SmithEnhanceSubsystem.generated.h"

class USmithBattleLogWorldSubsystem;
/**
 * 
 */
UCLASS()
class SMITHGAMEPLAY_API USmithEnhanceSubsystem : public UWorldSubsystem, public IEnhanceSystem
{
	GENERATED_BODY()

public:
	//---Begin of USubsystem Interface
	bool ShouldCreateSubsystem(UObject* Outer) const override final;
	void Initialize(FSubsystemCollectionBase& Collection) override final;
	void Deinitialize() override final;
	//---End of USubsystem Interface

public:

	void Enhance(IEnhanceable*, IParamAbsorbable*) override final;
	int32 GetUpgradeCount() const;

private:

	UPROPERTY()
	TObjectPtr<USmithBattleLogWorldSubsystem> m_logSystem;

	int32 m_upgradeCount;
};
