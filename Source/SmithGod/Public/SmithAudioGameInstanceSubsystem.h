// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IAudioPlayer.h"
#include "SmithAudioGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGOD_API USmithAudioGameInstanceSubsystem : public UGameInstanceSubsystem, public IAudioPlayer
{
	GENERATED_BODY()

public:
	FSoundEffectHandle PlaySE3D(const FString&, float volume = 1.0f, const FVector& location = FVector::ZeroVector);
	FSoundEffectHandle PlaySE2D(const FString&, float volume = 1.0f);
	bool StopSE(const FSoundEffectHandle&);
	void PlayBGM(const FString&, float volume = 1.0f);
	bool StopBGM();

private:
};
