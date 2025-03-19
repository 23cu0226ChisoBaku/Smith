// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SmithBattleLogWorldSubsystem.generated.h"

class UGameLogWidget;
class ISmithBattleLogger;
class ISmithEventLogger;

enum class EBattleLogType : uint8;

UCLASS()
class SMITHGOD_API USmithBattleLogWorldSubsystem final : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	//---Begin of USubsystem Interface
	bool ShouldCreateSubsystem(UObject* Outer) const override final;
	void Initialize(FSubsystemCollectionBase& Collection) override final;
	void Deinitialize() override final;
	//---End of USubsystem Interface

public:

	void SetLogWidget(UGameLogWidget*);
	void SendAttackLog(ISmithBattleLogger* attacker, ISmithBattleLogger* defender);
	void SendDamageLog(ISmithBattleLogger* defender, int32 damage);
	void SendDefeatedLog(ISmithBattleLogger* downed);
	void SendInteractEventLog(ISmithBattleLogger* interacter, ISmithEventLogger* event, bool bIsInteractSuccess);
	void SendEnhanceLog(ISmithBattleLogger* enhance);

private:
	void convertLogColor(FString& outLog, EBattleLogType);

private:

	UPROPERTY()
	TObjectPtr<UGameLogWidget> m_logWidget;
};
