// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "SmithMapModel.h"

#include "ISmithMapModelRequester.generated.h"


UINTERFACE(MinimalAPI)
class USmithMapModelRequester : public UInterface
{
	GENERATED_BODY()
};


class SMITHMODEL_API ISmithMapModelRequester
{
	GENERATED_BODY()

public:
	virtual const FSmithMapModel GetModel(AActor* Requester) const = 0;
};
