// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithBattleLogModelRepository.h"

#include "SmithBattleLogModelDefinition.h"
#include "SmithTurnBattleWorldSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithBattleLogModelRepository)

bool USmithBattleLogModelRepository::ShouldCreateSubsystem(UObject* Outer) const
{
  if (!Super::ShouldCreateSubsystem(Outer))
  {
    return false;
  }

  UWorld* worldOuter = Cast<UWorld>(Outer);
  if (worldOuter != nullptr)
  {
    // バトルレベルだけ作る
    ASmithTurnBattleWorldSettings* smithWorldSettings = Cast<ASmithTurnBattleWorldSettings>(worldOuter->GetWorldSettings());
    if (smithWorldSettings != nullptr)
    {
      return smithWorldSettings->IsBattleLevel();
    }
  }

  return false;
}

void USmithBattleLogModelRepository::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  m_models.Reset();
}

void USmithBattleLogModelRepository::Deinitialize()
{
  m_models.Reset();

  Super::Deinitialize();
}

void USmithBattleLogModelRepository::InitializeBattleLogModel(USmithBattleLogModelDefinition* DefinitionAsset)
{
  // check(DefinitionAsset != nullptr);
  // check(DefinitionAsset->LogClass != nullptr);

  // if (m_models.Contains(DefinitionAsset->LogClass))
  // {
  //   return;
  // }

  // FSmithBattleLogModel model = FSmithBattleLogModel::CreateModel(DefinitionAsset);
  // m_models.Add({DefinitionAsset->LogClass, model});
}

const FSmithBattleLogModel USmithBattleLogModelRepository::GetModel(UObject* Requester) const
{
  check(Requester != nullptr);

  // モデル請求者のUClassが登録されていれば有効モデルを返す
  UClass* requesterClass = Requester->GetClass();
  if (m_models.Contains(requesterClass))
  {
    return m_models[requesterClass];
  }

  return FSmithBattleLogModel{};
}
