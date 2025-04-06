// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleLogWorldSubsystem.h"

#include "GameLogWidget.h"
#include "ISmithBattleLogModelRequester.h"
#include "ISmithEventLogger.h"
#include "ISmithEventModelRequester.h"
#include "MLibrary.h"
#include "SmithTurnBattleWorldSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithBattleLogWorldSubsystem)

bool USmithBattleLogWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
  if (!Super::ShouldCreateSubsystem(Outer))
  {
    return false;
  }

  UWorld* worldOuter = Cast<UWorld>(Outer);
  if (::IsValid(worldOuter))
  {
    ASmithTurnBattleWorldSettings* smithWorldSettings = Cast<ASmithTurnBattleWorldSettings>(worldOuter->GetWorldSettings());
    if (::IsValid(smithWorldSettings))
    {
      return smithWorldSettings->IsBattleLevel();
    }
  }

  return false;
}

void USmithBattleLogWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  m_logWidget = nullptr;
  m_battleLogModelRequester = nullptr;
}

void USmithBattleLogWorldSubsystem::Deinitialize()
{
  if (m_logWidget != nullptr)
  {
    m_logWidget->SetVisibility(ESlateVisibility::Hidden);
    m_logWidget = nullptr;
  }

  m_battleLogModelRequester.Reset();
}

void USmithBattleLogWorldSubsystem::SetLogWidget(UGameLogWidget* logWidget)
{
  m_logWidget = logWidget;
  if (m_logWidget != nullptr)
  {
    m_logWidget->AddToViewport(-1);
    m_logWidget->SetVisibility(ESlateVisibility::Hidden);
  }
}

void USmithBattleLogWorldSubsystem::AssignLogRepository(ISmithBattleLogModelRequester* BattleLogRepository,
                                                        ISmithEventModelRequester* EventLogRepository)
{
  m_battleLogModelRequester = BattleLogRepository;
  m_eventModelRequester = EventLogRepository;
}

void USmithBattleLogWorldSubsystem::SendAttackLog(UObject* attacker, UObject* defender) const
{
  check(attacker != nullptr);
  check(defender != nullptr);
  if ((m_logWidget == nullptr) || !m_battleLogModelRequester.IsValid())
  {
    return;
  }

  const FSmithBattleLogModel attackerModel = m_battleLogModelRequester->GetModel(attacker);
  const FSmithBattleLogModel defenderModel = m_battleLogModelRequester->GetModel(defender);

  if (!attackerModel.IsValid() || !defenderModel.IsValid())
  {
    MDebug::LogWarning("Log Model is not initialized");
    return;
  }

  FString attackerLog = attackerModel.GetName();
  const EBattleLogModelType attackerLogType = attackerModel.GetType();
  
  FString defenderLog = defenderModel.GetName();
  const EBattleLogModelType defenderLogType = defenderModel.GetType();

  convertLogColor(attackerLog, attackerLogType);
  convertLogColor(defenderLog, defenderLogType);

  // TODO
  attackerLog.Append(TEXT("が"));
  defenderLog.Append(TEXT("に攻撃\n"));

  const FString resultLog = attackerLog + defenderLog;

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog(); 
}



void USmithBattleLogWorldSubsystem::SendDamageLog(UObject* defender, int32 damage) const
{
  check(defender != nullptr)
  if ((m_logWidget == nullptr) || !m_battleLogModelRequester.IsValid())
  {
    return;
  }

  const FSmithBattleLogModel defenderModel = m_battleLogModelRequester->GetModel(defender);
  if (!defenderModel.IsValid())
  {
    return;
  }

  FString defenderLog = defenderModel.GetName();
  const EBattleLogModelType defenderLogType = defenderModel.GetType();

  convertLogColor(defenderLog, defenderLogType);

  const FString damageNumLog = FString::FromInt(damage);
  const FString damageLog = TEXT("は") + damageNumLog + TEXT("ダメージを受けた\n");

  const FString resultLog = defenderLog + damageLog;

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();

}

void USmithBattleLogWorldSubsystem::SendDefeatedLog(UObject* downed) const
{
  check(downed != nullptr);
  if ((m_logWidget == nullptr) || !m_battleLogModelRequester.IsValid())
  {
    return;
  }

  const FSmithBattleLogModel downedModel = m_battleLogModelRequester->GetModel(downed);
  if (!downedModel.IsValid())
  {
    return;
  }

  FString downedLog = downedModel.GetName();
  const EBattleLogModelType downedLogType = downedModel.GetType();

  convertLogColor(downedLog, downedLogType);

  const FString defeatedLog = TEXT("が倒れた\n");
  const FString resultLog = downedLog + defeatedLog;

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();

}

void USmithBattleLogWorldSubsystem::SendInteractEventLog(UObject* Instigator, ISmithEventLogger* Event, bool bIsInteractSuccess) const
{
  check(Instigator != nullptr);
  check(Event != nullptr);

  if ((m_logWidget == nullptr) || !m_battleLogModelRequester.IsValid() || !m_eventModelRequester.IsValid())
  {
    return;
  }

  const FSmithBattleLogModel instigatorModel = m_battleLogModelRequester->GetModel(Instigator);
  const FSmithBattleLogModel eventEntityModel = m_battleLogModelRequester->GetModel(Event->GetEventEntity());
  if (!instigatorModel.IsValid() || !eventEntityModel.IsValid())
  {
    return;
  }

  const FSmithEventModel eventResultModel = m_eventModelRequester->GetModel(Event->_getUObject());
  if (!eventResultModel.IsValid())
  {
    return;
  }

  FString instigatorName = instigatorModel.GetName();
  const EBattleLogModelType instigatorType = instigatorModel.GetType();

  convertLogColor(instigatorName, instigatorType);

  FString eventEntityName = eventEntityModel.GetName();
  const EBattleLogModelType eventEntityType = eventEntityModel.GetType();
  convertLogColor(eventEntityName, eventEntityType);

  const FString eventResultLog = bIsInteractSuccess ? eventResultModel.GetSucceededMsg() : eventResultModel.GetFailedMsg();
  const FString resultLog = instigatorName + TEXT("は") + eventEntityName + eventResultLog + TEXT("\n"); 

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();
}

void USmithBattleLogWorldSubsystem::SendEnhanceLog(UObject* EnhanceEntity) const
{
  check(EnhanceEntity != nullptr)
  if ((m_logWidget == nullptr) || !m_battleLogModelRequester.IsValid())
  {
    return;
  }

  const FSmithBattleLogModel enhanceModel = m_battleLogModelRequester->GetModel(EnhanceEntity);
  if (!enhanceModel.IsValid())
  {
    return;
  }

  FString enhanceName = enhanceModel.GetName();
  const EBattleLogModelType enhanceType = enhanceModel.GetType();
  convertLogColor(enhanceName, enhanceType);

  const FString resultLog = enhanceName + TEXT("を強化した。\n");

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();
}

void USmithBattleLogWorldSubsystem::convertLogColor(FString& outLog, EBattleLogModelType logType) const
{
  using enum EBattleLogModelType;
  switch (logType)
  {
    case Player:
    case Enhance:
    {
      outLog = TEXT("<LogColorStyle.Player>") + outLog;
    }
    break;
    case Enemy:
    {
      outLog = TEXT("<LogColorStyle.Enemy>") + outLog;
    }
    break;
    case Item:
    {
      outLog = TEXT("<LogColorStyle.Item>") + outLog;
    }
    break;
    default:
    {
      outLog = TEXT("<LogColorStyle.None>") + outLog;
    }
    break;
  }

  outLog.Append(TEXT("</>"));

}