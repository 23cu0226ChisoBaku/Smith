// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "gameover_test.generated.h"

DECLARE_DELEGATE(FGameOverEventDispatcher);

/**
 * 
 */
UCLASS()
class SMITH_API Ugameover_test : public UObject
{
	GENERATED_BODY()

public:

	void TriggerEvent();

	FGameOverEventDispatcher GameOverEventdispatcher;
	
};
