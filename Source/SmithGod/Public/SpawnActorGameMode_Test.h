// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpawnActorGameMode_Test.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGOD_API ASpawnActorGameMode_Test : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpawnActorGameMode_Test();
	virtual void StartPlay() override;

};
