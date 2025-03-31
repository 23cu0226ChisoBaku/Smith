// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapModelRepository.h"

#include "SmithMapModelDefinition.h"
#include "SmithTurnBattleWorldSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithMapModelRepository)

bool USmithMapModelRepository::ShouldCreateSubsystem(UObject* Outer) const
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

void USmithMapModelRepository::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  m_models.Reset();
}

void USmithMapModelRepository::Deinitialize()
{
  m_models.Reset();

  Super::Deinitialize();
}

void USmithMapModelRepository::InitializeMapModel(USmithMapModelDefinition* DefinitionAsset)
{
  check(DefinitionAsset != nullptr);
  check(DefinitionAsset->MapObjectActorClass != nullptr);

  if (m_models.Contains(DefinitionAsset->MapObjectActorClass))
  {
    return;
  }

  FSmithMapModel newModel = FSmithMapModel::CreateModel(DefinitionAsset);
  check(newModel.IsValid());

  m_models.Add({DefinitionAsset->MapObjectActorClass, newModel});

}

const FSmithMapModel USmithMapModelRepository::GetModel(AActor* Requester) const
{
  check(Requester != nullptr);

  TSubclassOf<AActor> requesterClass = Requester->GetClass();
  if (m_models.Contains(requesterClass))
  {
    return m_models[requesterClass];
  }

  // TODO
  return FSmithMapModel::CreateModel(nullptr);

}