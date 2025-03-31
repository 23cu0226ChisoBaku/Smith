// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "UObject/WeakInterfacePtr.h"

#include "SmithBattleLogWorldSubsystem.generated.h"

class UGameLogWidget;
class ISmithEventLogger;
class ISmithBattleLogModelRequester;
class ISmithEventModelRequester;

enum class EBattleLogModelType : uint8;

UCLASS()
class SMITH_API USmithBattleLogWorldSubsystem final : public UWorldSubsystem
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
	void AssignLogRepository(ISmithBattleLogModelRequester* BattleLogRepository, ISmithEventModelRequester* EventLogRepository);
	void SendAttackLog(UObject* attacker, UObject* defender);
	void SendDamageLog(UObject* defender, int32 damage);
	void SendDefeatedLog(UObject* downed);
	void SendInteractEventLog(UObject* interacter, ISmithEventLogger* event, bool bIsInteractSuccess);
	void SendEnhanceLog(UObject* enhance);

private:
	void convertLogColor(FString& outLog, EBattleLogModelType);

private:

	UPROPERTY()
	TObjectPtr<UGameLogWidget> m_logWidget;

	TWeakInterfacePtr<ISmithBattleLogModelRequester> m_battleLogModelRequester;

	TWeakInterfacePtr<ISmithEventModelRequester> m_eventModelRequester;
};
