// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "ISmithMapModelRequester.h"

#include "SmithMapModelRepository.generated.h"

class IMapModelGateway;


/// @brief		ダンジョンモデルリポジトリ
UCLASS()
class SMITHMODEL_API USmithMapModelRepository : public UWorldSubsystem , public ISmithMapModelRequester
{
	GENERATED_BODY()
	
public:

	//---Begin of USubsystem Interface
	bool ShouldCreateSubsystem(UObject* Outer) const override final;
	void Initialize(FSubsystemCollectionBase& Collection) override final;
	void Deinitialize() override final;
	//---End of USubsystem Interface

	void InitializeMapModel(IMapModelGateway* ModelMapper);

	//---Begin of ISmithMapModelRequester Interface
	const FSmithMapModel GetModel(AActor* Requester) const override; 
	//---End of ISmithMapModelRequester Interface

private:

	TMap<TSubclassOf<AActor>, const FSmithMapModel> m_models;
};
