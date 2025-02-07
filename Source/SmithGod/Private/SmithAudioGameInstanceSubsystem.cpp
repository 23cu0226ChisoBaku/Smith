// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithAudioGameInstanceSubsystem.h"

FSoundEffectHandle USmithAudioGameInstanceSubsystem::PlaySE3D(const FString&, float volume, const FVector& location)
{
  return FSoundEffectHandle{};
}
FSoundEffectHandle USmithAudioGameInstanceSubsystem::PlaySE2D(const FString&, float volume)
{
  return FSoundEffectHandle{};
}
bool USmithAudioGameInstanceSubsystem::StopSE(const FSoundEffectHandle&)
{
  return false;
}

void USmithAudioGameInstanceSubsystem::PlayBGM(const FString&, float volume)
{

}
bool USmithAudioGameInstanceSubsystem::StopBGM()
{
  return false;
}