// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "UObject/WeakInterfacePtr.h"
#include "ISmithLogSender.h"

#include "SmithBattleLogWorldSubsystem.generated.h"

class UGameLogWidget;
class ISmithBattleLogModelRequester;
class ISmithEventModelRequester;

enum class EBattleLogModelType : uint8;

UCLASS()
class SMITHGAMEPLAY_API USmithBattleLogWorldSubsystem final : public UWorldSubsystem , public ISmithLogSender
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
	void SendAttackLog(UObject* attacker, UObject* defender) const override;
	void SendDamageLog(UObject* defender, int32 damage) const override;
	void SendDefeatedLog(UObject* downed) const override;
	void SendInteractEventLog(UObject* interacter, ISmithEventLogger* event, bool bIsInteractSuccess) const override;
	void SendEnhanceLog(UObject* enhance) const override;

private:
	void convertLogColor(FString& outLog, EBattleLogModelType) const;

private:

	UPROPERTY()
	TObjectPtr<UGameLogWidget> m_logWidget;

	TWeakInterfacePtr<ISmithBattleLogModelRequester> m_battleLogModelRequester;

	TWeakInterfacePtr<ISmithEventModelRequester> m_eventModelRequester;
};
