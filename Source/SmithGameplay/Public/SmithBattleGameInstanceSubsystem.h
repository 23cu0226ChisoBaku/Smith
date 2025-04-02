// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SmithBattleGameInstanceSubsystem.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class SMITHGAMEPLAY_API USmithBattleGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void DisplayGameOverWidget(UObject* worldContextObject);
	UFUNCTION(BlueprintCallable)
	void DisplayGameClearWidget(UObject* worldContextObject);
};
