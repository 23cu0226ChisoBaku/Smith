// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "IEventModelGateway.generated.h"

struct SMITHMODEL_API FEventModelDTO
{
	TSubclassOf<UObject> EventClass = nullptr;
	FString SucceededMsg;
	FString FailedMsg;
};

bool SMITHMODEL_API operator==(const FEventModelDTO& Lhs, const FEventModelDTO& Rhs);

UINTERFACE(MinimalAPI)
class UEventModelGateway : public UInterface
{
	GENERATED_BODY()
};

class SMITHMODEL_API IEventModelGateway
{
	GENERATED_BODY()

public:

	virtual int32 ReadAll(TArray<FEventModelDTO>& ModelDTOs) const = 0;
};
