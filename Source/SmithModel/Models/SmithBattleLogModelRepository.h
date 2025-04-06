// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "ISmithBattleLogModelRequester.h"

#include "SmithBattleLogModelRepository.generated.h"

class IBattleLogModelGateway;

/// @brief  バトルログモデルリポジトリ
UCLASS()
class SMITHMODEL_API USmithBattleLogModelRepository : public UWorldSubsystem , public ISmithBattleLogModelRequester
{
  GENERATED_BODY()

public:
  //---Begin of USubsystem Interface
  bool ShouldCreateSubsystem(UObject* Outer) const override final;
  void Initialize(FSubsystemCollectionBase& Collection) override final;
  void Deinitialize() override final;
  //---End of USubsystem Interface

  void InitializeBattleLogModel(IBattleLogModelGateway* ModelMapper);

  //---Begin of ISmithBattleLogModelRequester
  const FSmithBattleLogModel GetModel(UObject* Requester) const override;
  //---End of ISmithBattleLogModelRequester

  private:

    TMap<TSubclassOf<UObject>, const FSmithBattleLogModel> m_models;
  
};
