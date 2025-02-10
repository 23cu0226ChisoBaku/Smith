// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "IEnhanceSystem.h"
#include "SmithEnhanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithEnhanceSubsystem : public UWorldSubsystem, public IEnhanceSystem
{
	GENERATED_BODY()

public:
	bool ShouldCreateSubsystem(UObject* Outer) const override final;
	/** Implement this for initialization of instances of the system */
	void Initialize(FSubsystemCollectionBase& Collection) override final;
	/** Implement this for deinitialization of instances of the system */
	void Deinitialize() override final;

public:
	void Enhance(IEnhanceable*, IParamAbsorbable*) override final;
	int32 GetUpgradeCount() const;

private:
	int32 m_upgradeCount;
};
