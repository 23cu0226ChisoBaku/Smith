// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITurnManageable.generated.h"

class UTurnControlComponent;
class IBattleCommand;
class ITurnManageable;

// コマンドを出す専用デリゲート
DECLARE_MULTICAST_DELEGATE_TwoParams(FRequestCommandEvent, ITurnManageable*, TSharedPtr<IBattleCommand>);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTurnManageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SMITH_API ITurnManageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UTurnControlComponent* GetTurnControl() const = 0;
	virtual FDelegateHandle Subscribe(FRequestCommandEvent::FDelegate&) = 0;
	virtual bool Unsubscribe(UObject*,FDelegateHandle) = 0;
};