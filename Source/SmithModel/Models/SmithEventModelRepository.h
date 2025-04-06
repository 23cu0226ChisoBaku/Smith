// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "ISmithEventModelRequester.h"

#include "SmithEventModelRepository.generated.h"

class IEventModelGateway;

/// @brief  イベントモデルリポジトリ
UCLASS()
class SMITHMODEL_API USmithEventModelRepository : public UWorldSubsystem , public ISmithEventModelRequester
{
	GENERATED_BODY()
	
	public:
  //---Begin of USubsystem Interface
  bool ShouldCreateSubsystem(UObject* Outer) const override final;
  void Initialize(FSubsystemCollectionBase& Collection) override final;
  void Deinitialize() override final;
  //---End of USubsystem Interface

  void InitializeEventModel(IEventModelGateway* ModelMapper);

  //---Begin of ISmithEventModelRequester Interface
  const FSmithEventModel GetModel(UObject* Requester) const override;
  //---End of ISmithEventModelRequester Interface

  private:

    TMap<TSubclassOf<UObject>, const FSmithEventModel> m_models;
};
