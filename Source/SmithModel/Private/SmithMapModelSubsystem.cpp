// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapModelSubsystem.h"

#include "SmithMapModelDefinition.h"
#include "SmithTurnBattleWorldSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithMapModelSubsystem)

bool USmithMapModelSubsystem::ShouldCreateSubsystem(UObject* Outer) const
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

void USmithMapModelSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  m_mapModels.Reset();
}

void USmithMapModelSubsystem::Deinitialize()
{
  m_mapModels.Reset();

  Super::Deinitialize();
}

void USmithMapModelSubsystem::InitializeMapModel(USmithMapModelDefinition* DefinitionAsset)
{
  check(DefinitionAsset != nullptr);
  check(DefinitionAsset->MapObjectActorClass != nullptr);

  if (m_mapModels.Contains(DefinitionAsset->MapObjectActorClass))
  {
    return;
  }

  FSmithMapModel newModel = FSmithMapModel::CreateModel(DefinitionAsset);
  check(newModel.IsValid());

  m_mapModels.Add({DefinitionAsset->MapObjectActorClass, newModel});

}

FSmithMapModel USmithMapModelSubsystem::GetModel(AActor* Requester) const
{
  check(Requester != nullptr);

  TSubclassOf<AActor> requesterClass = Requester->GetClass();
  if (m_mapModels.Contains(requesterClass))
  {
    return m_mapModels[requesterClass];
  }

  // TODO
  return FSmithMapModel::CreateModel(nullptr);

}