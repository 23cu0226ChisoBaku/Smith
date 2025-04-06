// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "Params.h"

#include "SmithBattleParamModelRepository.generated.h"

class IBattleParamModelGateway;
struct FParams;

UCLASS()
class SMITHMODEL_API USmithBattleParamModelRepository : public UWorldSubsystem
{
  GENERATED_BODY()

  public:
    //---Begin of USubsystem Interface
    bool ShouldCreateSubsystem(UObject* Outer) const override final;
    void Initialize(FSubsystemCollectionBase& Collection) override final;
    void Deinitialize() override final;
    //---End of USubsystem Interface

    void InitializeBattleParamModel(IBattleParamModelGateway* ModelMapper);

    const FParams GetModel(UObject* Requester) const;
  
  private:
    
    TMap<TSubclassOf<UObject>, const FParams> m_models;
  
};
