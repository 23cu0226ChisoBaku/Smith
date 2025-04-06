// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithMapModelRepository.h"

#include "IMapModelGateway.h"
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

void USmithMapModelRepository::InitializeMapModel(IMapModelGateway* ModelMapper)
{
  check(ModelMapper != nullptr);

  TArray<FMapModelDTO> modelDTOs{};
  int32 dtoCount = ModelMapper->ReadAll(modelDTOs);
  if (dtoCount <= 0)
  {
    return;
  }
  
  for(const FMapModelDTO& DTO : modelDTOs)
  {
    UClass* modelClass = DTO.ActorClass;
    if ((modelClass == nullptr) || m_models.Contains(modelClass))
    {
      continue;
    }

    FSmithMapModel newModel = FSmithMapModel::CreateModel(DTO.ActorClass, DTO.SizeX, DTO.SizeY, DTO.Type);

    m_models.Add({modelClass, newModel});
  }
}

const FSmithMapModel USmithMapModelRepository::GetModel(AActor* Requester) const
{
  check(Requester != nullptr);

  // モデル請求者のUClassが登録されていれば有効モデルを返す
  TSubclassOf<AActor> requesterClass = Requester->GetClass();
  if (m_models.Contains(requesterClass))
  {
    return m_models[requesterClass];
  }

  return FSmithMapModel{};

}