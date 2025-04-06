// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IBattleParamModelGateway.generated.h"

struct SMITHMODEL_API FBattleParamDTO
{
	TSubclassOf<UObject> ParamClass = nullptr;
	int32 HitPoints = 0;
	int32 Attack = 0;
	int32 Defense = 0;
};

bool SMITHMODEL_API operator==(const FBattleParamDTO& Lhs, const FBattleParamDTO& Rhs);

UINTERFACE(MinimalAPI)
class UBattleParamModelGateway : public UInterface
{
	GENERATED_BODY()
};

class SMITHMODEL_API IBattleParamModelGateway
{
	GENERATED_BODY()

public:
	virtual int32 ReadAll(TArray<FBattleParamDTO>& ModelDTOs) const = 0;
};
