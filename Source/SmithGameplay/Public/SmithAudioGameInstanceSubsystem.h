// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IAudioPlayer.h"
#include "Tickable.h"
#include "SmithAudioGameInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SMITHGAMEPLAY_API USmithAudioGameInstanceSubsystem : public UGameInstanceSubsystem, public FTickableGameObject,
																											public IAudioPlayer
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();

	UFUNCTION(BlueprintCallable)
	void InitializeSounds(UDataTable* bgmTable, UDataTable* seTable);
	FSoundEffectHandle PlaySE3D(const FString& seName, float volume = 1.0f, const FVector& location = FVector::ZeroVector);
	FSoundEffectHandle PlaySE(const FString& seName, float volume = 1.0f);
	bool StopSE(const FSoundEffectHandle& seHandle);
	void PlayBGM(const FString&, float volume = 1.0f);
	bool StopBGM();

public:
	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	UPROPERTY()
	TMap<FString, TSoftObjectPtr<USoundBase>> m_bgmBuffer;
	UPROPERTY()
	TMap<FString, TSoftObjectPtr<USoundBase>> m_seBuffer;
	UPROPERTY()
	TObjectPtr<UAudioComponent> m_bgmComp;

private:
	TArray<FSoundEffectHandle> m_seHandle;
	TMap<FString, FString> m_seIdentifier;
	uint8 m_bIsTickable : 1;
};
