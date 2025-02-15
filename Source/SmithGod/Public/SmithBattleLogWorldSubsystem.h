// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SmithBattleLogWorldSubsystem.generated.h"

class UGameLogWidget;
class ISmithBattleLogger;
class ISmithEventLogger;

enum class EBattleLogType : uint8;
/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithBattleLogWorldSubsystem final: public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	bool ShouldCreateSubsystem(UObject* Outer) const override final;

/** Implement this for initialization of instances of the system */
	void Initialize(FSubsystemCollectionBase& Collection) override final;
	/** Implement this for deinitialization of instances of the system */
	void Deinitialize() override final;

	// start of FTickableObjectBase Interface
#pragma region FTickableObjectBase Interface
	void Tick(float DeltaTime) override final;
	bool IsTickable() const override final;
	TStatId GetStatId() const override final;
#pragma endregion
// end of FTickableObjectBase Interface

// start of FTickableObject Interface
#pragma region FTickableObject Interface
	bool IsTickableWhenPaused() const override final;
	bool IsTickableInEditor() const override final;
	UWorld* GetTickableGameObjectWorld() const override final;
#pragma endregion
// end of FTickableObject Interface
public:
	void SetLogWidget(UGameLogWidget*);
	void SendAttackLog(ISmithBattleLogger* attacker, ISmithBattleLogger* defender);
	void SendDamageLog(ISmithBattleLogger* defender, int32 damage);
	void SendDefeatedLog(ISmithBattleLogger* downed);
	void SendInteractEventLog(ISmithBattleLogger* interacter, ISmithEventLogger* event, bool bIsInteractSuccess);
	void SendEnhanceLog(ISmithBattleLogger* enhancer);

private:
	void convertLogColor(FString& outLog, EBattleLogType);
private:
	UPROPERTY()
	TObjectPtr<UGameLogWidget> m_logWidget;
};
