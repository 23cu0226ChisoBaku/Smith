// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "SmithTurnBattleWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class SMITH_API ASmithTurnBattleWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bUseBattleSubSystem = false;

public:
	bool IsBattleLevel() const;
};
