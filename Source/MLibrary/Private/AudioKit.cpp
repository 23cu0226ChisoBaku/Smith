// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioKit.h"
#include "IAudioPlayer.h"

namespace MLibrary
{
	namespace UE::Audio
	{
    void AudioKit::AssignAudioPlayer(IAudioPlayer*)
    {

    }
    FSoundEffectHandle AudioKit::PlaySE3D(const FString&, float volume, const FVector& location)
    {
      return FSoundEffectHandle{};
    }
    FSoundEffectHandle AudioKit::PlaySE2D(const FString&, float volume)
    {
      return FSoundEffectHandle{};
    }
    bool AudioKit::StopSE(const FSoundEffectHandle&)
    {
      return false;
    }
    void AudioKit::PlayBGM(const FString&, float volume)
    {

    }
    bool AudioKit::StopBGM()
    {
      return false;
    }
  }
}

