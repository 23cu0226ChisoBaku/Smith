// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEnhanceSubsystem.h"

#include "SmithTurnBattleWorldSettings.h"
#include "IEnhanceable.h"
#include "ParamAbsorbable.h"

#include "SmithBattleLogWorldSubsystem.h"


bool USmithEnhanceSubsystem::ShouldCreateSubsystem(UObject* Outer) const
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

void USmithEnhanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);
  Collection.InitializeDependency(USmithBattleLogWorldSubsystem::StaticClass());
  m_logSystem = GetWorld()->GetSubsystem<USmithBattleLogWorldSubsystem>();
  
  m_upgradeCount = 0;
}

void USmithEnhanceSubsystem::Deinitialize()
{
  Super::Deinitialize();
}

void USmithEnhanceSubsystem::Enhance(IEnhanceable* enhanceable, IParamAbsorbable* absorbItem)
{
  if ((enhanceable == nullptr) || (absorbItem == nullptr))
  {
    return;
  }

  enhanceable->Enhance(absorbItem);

  if (m_logSystem != nullptr)
  {
    m_logSystem->SendEnhanceLog(enhanceable->_getUObject());
  }
  
  enhanceable->OnEnhanced();
  ++m_upgradeCount;

}

int32 USmithEnhanceSubsystem::GetUpgradeCount() const
{
  return m_upgradeCount;
}
