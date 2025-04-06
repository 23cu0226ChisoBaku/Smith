// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithEventModelRepository.h"

#include "IEventModelGateway.h"
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

void USmithEventModelRepository::InitializeEventModel(IEventModelGateway* ModelMapper)
{
  check(ModelMapper != nullptr);

  TArray<FEventModelDTO> modelDTOs{};
  int32 dtoCount = ModelMapper->ReadAll(modelDTOs);
  if (dtoCount <= 0)
  {
    return;
  }

  for (const FEventModelDTO& dto : modelDTOs)
  {
    UClass* eventClass = dto.EventClass;
    if ((eventClass == nullptr) || m_models.Contains(eventClass))
    {
      continue;
    }

    FSmithEventModel newModel = FSmithEventModel::CreateModel(dto.EventClass, dto.SucceededMsg, dto.FailedMsg);
    m_models.Add({eventClass, newModel});
  }
}

const FSmithEventModel USmithEventModelRepository::GetModel(UObject* Requester) const
{
  check(Requester != nullptr);

  // モデル請求者のUClassが登録されていれば有効モデルを返す
  UClass* requesterClass = Requester->GetClass();
  if (m_models.Contains(requesterClass))
  {
    return m_models[requesterClass];
  }

  return FSmithEventModel{};
}


