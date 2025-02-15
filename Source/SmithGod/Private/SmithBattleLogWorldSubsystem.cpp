// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleLogWorldSubsystem.h"
#include "SmithTurnBattleWorldSettings.h"
#include "GameLogWidget.h"
#include "ISmithBattleLogger.h"
#include "ISmithEventLogger.h"

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
}

void USmithBattleLogWorldSubsystem::Deinitialize()
{
  if (m_logWidget != nullptr)
  {
    m_logWidget->SetVisibility(ESlateVisibility::Hidden);
    m_logWidget = nullptr;
  }
}

void USmithBattleLogWorldSubsystem::Tick(float DeltaTime)
{

}

bool USmithBattleLogWorldSubsystem::IsTickable() const
{
  return true;
}

TStatId USmithBattleLogWorldSubsystem::GetStatId() const
{
  RETURN_QUICK_DECLARE_CYCLE_STAT(USmithBattleLogWorldSubsystem, STATGROUP_Tickables);
}

bool USmithBattleLogWorldSubsystem::IsTickableWhenPaused() const
{
  return true;
}
bool USmithBattleLogWorldSubsystem::IsTickableInEditor() const
{
  return true;
}
UWorld* USmithBattleLogWorldSubsystem::GetTickableGameObjectWorld() const
{
  return GetWorld();
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

void USmithBattleLogWorldSubsystem::SendAttackLog(ISmithBattleLogger* attacker, ISmithBattleLogger* defender)
{
  if (m_logWidget == nullptr)
  {
    return;
  }

  FString attackerLog = attacker != nullptr ? attacker->GetName_Log() : TEXT("とある人物");
  const EBattleLogType attackerLogType = attacker != nullptr ? attacker->GetType_Log() : EBattleLogType::None;
  FString defenderLog = defender != nullptr ? defender->GetName_Log() : TEXT("とある対象");
  const EBattleLogType defenderLogType = defender != nullptr ? defender->GetType_Log() : EBattleLogType::None;

  convertLogColor(attackerLog, attackerLogType);
  convertLogColor(defenderLog, defenderLogType);

  attackerLog.Append(TEXT("が"));
  defenderLog.Append(TEXT("に攻撃\n"));

  const FString resultLog = attackerLog + defenderLog;

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog(); 
}



void USmithBattleLogWorldSubsystem::SendDamageLog(ISmithBattleLogger* defender, int32 damage)
{
  if (m_logWidget == nullptr)
  {
    return;
  }

  FString defenderLog = defender != nullptr ? defender->GetName_Log() : TEXT("とある対象");
  const EBattleLogType defenderLogType = defender != nullptr ? defender->GetType_Log() : EBattleLogType::None;

  convertLogColor(defenderLog, defenderLogType);
  const FString damageLog = TEXT("は") + FString::FromInt(damage) + TEXT("ダメージを受けた\n");

  const FString resultLog = defenderLog + damageLog;
  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();

}

void USmithBattleLogWorldSubsystem::SendDefeatedLog(ISmithBattleLogger* downed)
{
  if (m_logWidget == nullptr)
  {
    return;
  }

  FString downedLog = downed != nullptr ? downed->GetName_Log() : TEXT("とある対象");
  const EBattleLogType downedLogType = downed != nullptr ? downed->GetType_Log() : EBattleLogType::None;

  convertLogColor(downedLog, downedLogType);

  const FString defeatedLog = TEXT("が倒れた\n");
  const FString resultLog = downedLog + defeatedLog;

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();

}

void USmithBattleLogWorldSubsystem::SendInteractEventLog(ISmithBattleLogger* interacter, ISmithEventLogger* event, bool bIsInteractSuccess)
{
  if (m_logWidget == nullptr)
  {
    return;
  }

  if (event == nullptr)
  {
    return;
  }

  FString interacterName = interacter != nullptr ? interacter->GetName_Log() : TEXT("とある人物");
  const EBattleLogType interacterType = interacter != nullptr ? interacter->GetType_Log() : EBattleLogType::None;

  convertLogColor(interacterName, interacterType);

  ISmithBattleLogger* eventEntityLogger = event->GetEventEntityLogger();
  FString eventEntityName = eventEntityLogger != nullptr ? eventEntityLogger->GetName_Log() : TEXT("とあるもの");
  const EBattleLogType eventEntityType = eventEntityLogger != nullptr ? eventEntityLogger->GetType_Log() : EBattleLogType::None;
  convertLogColor(eventEntityName, eventEntityType);
  const FString eventResultLog = bIsInteractSuccess ? event->GetSucceedMessage() : event->GetFailedMessage();

  const FString resultLog = interacterName + TEXT("は") + eventEntityName + eventResultLog + TEXT("\n"); 

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();

}

void USmithBattleLogWorldSubsystem::SendEnhanceLog(ISmithBattleLogger* enhancer)
{
  if (m_logWidget == nullptr)
  {
    return;
  }

  FString enhanceLog = enhancer != nullptr ? enhancer->GetName_Log() : TEXT("とある対象");
  const EBattleLogType enhanceLogType = enhancer != nullptr ? enhancer->GetType_Log() : EBattleLogType::None;

  convertLogColor(enhanceLog, enhanceLogType);

  const FString enhancedLog = TEXT("が武器を強化した\n");
  const FString resultLog = enhanceLog + enhancedLog;

  m_logWidget->AddLogMessage(resultLog);
  m_logWidget->OutputLog();

}

void USmithBattleLogWorldSubsystem::convertLogColor(FString& outLog, EBattleLogType logType)
{
  switch (logType)
  {
    case EBattleLogType::Player:
    {
      outLog = TEXT("<LogColorStyle.Player>") + outLog;
    }
    break;
    case EBattleLogType::Enemy:
    {
      outLog = TEXT("<LogColorStyle.Enemy>") + outLog;
    }
    break;
    case EBattleLogType::Item:
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