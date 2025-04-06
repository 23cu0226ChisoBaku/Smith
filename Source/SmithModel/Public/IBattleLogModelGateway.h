// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "BattleLogType.h"

#include "IBattleLogModelGateway.generated.h"

struct SMITHMODEL_API FBattleLogModelDTO
{
	TSubclassOf<UObject> LogModelClass = nullptr;
	FString Name;
	EBattleLogModelType LogType = EBattleLogModelType::TypeCount;
};

bool SMITHMODEL_API operator==(const FBattleLogModelDTO& Lhs, const FBattleLogModelDTO& Rhs);

UINTERFACE(MinimalAPI)
class UBattleLogModelGateway : public UInterface
{
	GENERATED_BODY()
};

class SMITHMODEL_API IBattleLogModelGateway
{
	GENERATED_BODY()

public:

	virtual int32 ReadAll(TArray<FBattleLogModelDTO>& ModelDTOs) const = 0;
};
