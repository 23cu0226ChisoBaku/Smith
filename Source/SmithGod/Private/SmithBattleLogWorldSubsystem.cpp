// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleLogWorldSubsystem.h"
#include "SmithTurnBattleWorldSettings.h"
#include "GameLogWidget.h"

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
    m_logWidget->SetVisibility(ESlateVisibility::Hidden);
  }
}

void USmithBattleLogWorldSubsystem::SendAttackLog(ISmithBattleLogger* attacker, ISmithBattleLogger* defender)
{

}



void USmithBattleLogWorldSubsystem::SendDamageLog(ISmithBattleLogger* defender, int32 damage)
{

}

void USmithBattleLogWorldSubsystem::SendDefeatedLog(ISmithBattleLogger* downed)
{

}

void USmithBattleLogWorldSubsystem::SendInteractEventLog(ISmithBattleLogger* interacter, ISmithEventLogger* event, bool bIsInteractSuccess)
{
  
}