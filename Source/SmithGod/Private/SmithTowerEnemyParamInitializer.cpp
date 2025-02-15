// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithTowerEnemyParamInitializer.h"
#include "SmithEnemyParamInitializerRow.h"

USmithTowerEnemyParamInitializer::USmithTowerEnemyParamInitializer()
  : m_paramList{}
{
  
}

void USmithTowerEnemyParamInitializer::BeginDestroy()
{
  m_paramList.Empty();

  Super::BeginDestroy();
}

void USmithTowerEnemyParamInitializer::AssignEnemyParamList(UDataTable* paramList)
{
  if (paramList == nullptr)
  {
    return;
  }

  TArray<FSmithEnemyParamInitializerRow*> tableRows;
  paramList->GetAllRows<FSmithEnemyParamInitializerRow>(nullptr, tableRows);

  for (const auto& row : tableRows)
  {
    if (m_paramList.Contains(row->EnemyName))
    {
      continue;
    }

    m_paramList.Emplace(row->EnemyName, row->DefaultParam);
  }
}

FParams USmithTowerEnemyParamInitializer::Initialize(SmallGolem, int32 currentLevel)
{
  const FString tag = TEXT("SmallGolem");
  if (!m_paramList.Contains(tag))
  {
    return FParams{};
  }

  FParams returnParams = m_paramList[tag];
  returnParams.HP = returnParams.HP + FMath::FloorToInt(StaticCast<double>(returnParams.HP * (currentLevel - 1)) * 0.4);
  returnParams.ATK = returnParams.ATK + FMath::FloorToInt(StaticCast<double>(returnParams.ATK * (currentLevel - 1)) * 0.4);
  returnParams.DEF = returnParams.DEF + FMath::FloorToInt(StaticCast<double>(returnParams.DEF * (currentLevel - 1)) * 0.4);
  returnParams.CRT = returnParams.CRT + FMath::FloorToInt(StaticCast<double>(returnParams.CRT * (currentLevel - 1)) * 0.1);

  return returnParams;
}
FParams USmithTowerEnemyParamInitializer::Initialize(Dragon, int32 currentLevel)
{
  const FString tag = TEXT("Dragon");
  if (!m_paramList.Contains(tag))
  {
    return FParams{};
  }

  return m_paramList[tag];
}
FParams USmithTowerEnemyParamInitializer::Initialize(HerbGolem,int32 currentLevel)
{
  const FString tag = TEXT("HerbGolem");
  if (!m_paramList.Contains(tag))
  {
    return FParams{};
  }

  FParams returnParams = m_paramList[tag];
  returnParams.HP = returnParams.HP + FMath::FloorToInt(StaticCast<double>(returnParams.HP * (currentLevel - 1)) * 0.4);
  returnParams.ATK = returnParams.ATK + FMath::FloorToInt(StaticCast<double>(returnParams.ATK * (currentLevel - 1)) * 0.4);
  returnParams.DEF = returnParams.DEF + FMath::FloorToInt(StaticCast<double>(returnParams.DEF * (currentLevel - 1)) * 0.4);
  returnParams.CRT = returnParams.CRT + FMath::FloorToInt(StaticCast<double>(returnParams.CRT * (currentLevel - 1)) * 0.4);

  return returnParams;
}