// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SoundEffectHandle.h"
#include "AudioFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MLIBRARY_API UAudioFunctionLibrary : public UBlueprintFunctionLibrary
{
  GENERATED_BODY()

public:
  UFUNCTION(BlueprintCallable, Category = "Audio Kit")
  static void PlaySE3D(const FString& seName, float volume = 1.0f, const FVector& location = FVector::ZeroVector);
  UFUNCTION(BlueprintCallable, Category = "Audio Kit")
  static void PlaySE(const FString& seName, float volume = 1.0f);
  UFUNCTION(BlueprintCallable, Category = "Audio Kit")
  static void PlayBGM(const FString& bgmName, float volume = 1.0f);
  UFUNCTION(BlueprintCallable, Category = "Audio Kit")
  static void StopBGM();
  
};
