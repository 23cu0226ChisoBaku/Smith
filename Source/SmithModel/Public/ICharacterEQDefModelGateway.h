// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "ICharacterEQDefModelGateway.generated.h"

struct FCharaEquipmentDefDTO
{
	TSubclassOf<AActor> UserClass = nullptr;
	TArray<class USmithEquipmentDefinition> Definitions;
};

bool SMITHMODEL_API operator==(const FCharaEquipmentDefDTO& Lhs, const FCharaEquipmentDefDTO& Rhs);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterEQDefModelGateway : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITHMODEL_API ICharacterEQDefModelGateway
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int32 ReadAll(TArray<FCharaEquipmentDefDTO>& ModelDTOs) const = 0;
};
