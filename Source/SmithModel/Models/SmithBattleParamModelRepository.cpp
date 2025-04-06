// Fill out your copyright notice in the Description page of Project Settings.

#include "SmithBattleParamModelRepository.h"

#include "IBattleParamModelGateway.h"
#include "SmithTurnBattleWorldSettings.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SmithBattleParamModelRepository)

bool USmithBattleParamModelRepository::ShouldCreateSubsystem(UObject* Outer) const
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

void USmithBattleParamModelRepository::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  m_models.Reset();
}

void USmithBattleParamModelRepository::Deinitialize()
{
  m_models.Reset();

  Super::Deinitialize();
}

void USmithBattleParamModelRepository::InitializeBattleParamModel(IBattleParamModelGateway* ModelMapper)
{
  check(ModelMapper != nullptr);
  
  TArray<FBattleParamDTO> modelDTOs{};
  int32 dtoCount = ModelMapper->ReadAll(modelDTOs);
  if (dtoCount <= 0)
  {
    return;
  }

  for (const FBattleParamDTO& dto : modelDTOs)
  {
    UClass* paramClass = dto.ParamClass;
    if ((paramClass == nullptr) || m_models.Contains(paramClass))
    {
      continue;
    }

    FParams newDefaultParam{};
    newDefaultParam.HP = dto.HitPoints;
    newDefaultParam.ATK = dto.Attack;
    newDefaultParam.DEF = dto.Defense;
    newDefaultParam.CRT = 0;

    m_models.Emplace(paramClass, newDefaultParam);
  }
}

const FParams USmithBattleParamModelRepository::GetModel(UObject* Requester) const
{
  check(Requester != nullptr);

  UClass* requesterClass = Requester->GetClass();
  if (m_models.Contains(requesterClass))
  {
    return m_models[requesterClass];
  }

  return FParams{0, 0, 0, 0};
}



