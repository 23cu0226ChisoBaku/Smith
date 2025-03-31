// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEventModelRepository.h"

#include "SmithEventModelDefinition.h"
#include "SmithTurnBattleWorldSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithEventModelRepository)

bool USmithEventModelRepository::ShouldCreateSubsystem(UObject* Outer) const
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

void USmithEventModelRepository::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  m_models.Reset();
}

void USmithEventModelRepository::Deinitialize()
{
  m_models.Reset();

  Super::Deinitialize();
}

void USmithEventModelRepository::InitializeEventModel(USmithEventModelDefinition* DefinitionAsset)
{
  check(DefinitionAsset != nullptr);
  check(DefinitionAsset->EventClass != nullptr);

  UClass* eventClass = DefinitionAsset->EventClass;

  if (m_models.Contains(eventClass))
  {
    return;
  }

  FSmithEventModel model = FSmithEventModel::CreateModel(DefinitionAsset);
  m_models.Add({eventClass, model});

}

const FSmithEventModel USmithEventModelRepository::GetModel(UObject* Requester) const
{
  check(Requester != nullptr);

  UClass* requesterClass = Requester->GetClass();
  if (m_models.Contains(requesterClass))
  {
    return m_models[requesterClass];
  }

  return FSmithEventModel::CreateModel(nullptr);
}


