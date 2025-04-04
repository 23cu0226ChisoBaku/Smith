// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "SmithGameplay/Models/SmithBattleLogModel.h"

#include "ISmithBattleLogModelRequester.generated.h"

UINTERFACE(MinimalAPI)
class USmithBattleLogModelRequester : public UInterface
{
	GENERATED_BODY()
};

class SMITHMODEL_API ISmithBattleLogModelRequester
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual const FSmithBattleLogModel GetModel(UObject* Requester) const = 0;
};
