// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IMapModelGateway.generated.h"

struct FSmithMapModel;

UINTERFACE(MinimalAPI)
class UMapModelGateway : public UInterface
{
	GENERATED_BODY()
};

class SMITHMODEL_API IMapModelGateway
{
	GENERATED_BODY()

public:
	virtual int32 GetAllModelDatas(TArray<const FSmithMapModel>& Datas) const = 0;
};
