// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithLootGameInstanceSubsystem.h"

#include "SmithEnemyLootGenerator.h"
#include "SmithPickableDropRateListRow.h"

USmithLootGameInstanceSubsystem::USmithLootGameInstanceSubsystem()
  : m_lootList{}
{ }

void USmithLootGameInstanceSubsystem::BeginDestroy()
{
  m_lootList.Empty();
  Super::BeginDestroy();
}

void USmithLootGameInstanceSubsystem::AssignLootList(UDataTable* lootList)
{
  if (!::IsValid(lootList))
  {
    return;
  }

  TArray<FSmithPickableDropRateListRow*> enemyDropList;
  lootList->GetAllRows<FSmithPickableDropRateListRow>(nullptr, enemyDropList);
  if (enemyDropList.Num() == 0)
  {
    return;
  }

  m_lootList.Reset();

  for (const auto& dropList : enemyDropList)
  {
    if (!m_lootList.Contains(dropList->ListOwnerName))
    {
      m_lootList.Emplace(dropList->ListOwnerName, LootListElement{dropList->DropLootList, dropList->Mode});
    }
  }
}

IPickable* USmithLootGameInstanceSubsystem::GetLoot(const FString& EnemyTypeName) const
{
  if (!m_lootList.Contains(EnemyTypeName))
  {
    return nullptr;
  }

  float sumOfProbabilities = 0.0f;
  IPickable* pickable = nullptr;

  switch (m_lootList[EnemyTypeName].Mode)
  {
    case EDropRatioMode::Proportion:
    {
      for (const auto& lootHandle : m_lootList[EnemyTypeName].LootHandleList)
      {
        sumOfProbabilities += lootHandle.Ratio;
      }
    }
    break;

    case EDropRatioMode::Scalar:
    {
      sumOfProbabilities = 1.0f;     
    }
    break;

    default:
    {
      return nullptr;
    }
  }

  float random = FMath::RandRange(0.0f, sumOfProbabilities);
  for (const auto& lootHandle : m_lootList[EnemyTypeName].LootHandleList)
  {
    if (lootHandle.Ratio == 0.0f)
    {
      continue;
    }

    if (random <= lootHandle.Ratio)
    {
      if (lootHandle.LootSubclass != nullptr)
      {
        UObject* pickableObj = NewObject<UObject>(GetWorld(), lootHandle.LootSubclass);
        pickable = Cast<IPickable>(pickableObj);
        break;
      }
    }

    random -= lootHandle.Ratio;
  }

  return pickable;
}