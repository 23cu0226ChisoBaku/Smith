// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifndef MLIBRARY_AUDIO_KIT
#define MLIBRARY_AUDIO_KIT

#include "CoreMinimal.h"
#include "SoundEffectHandle.h"
#include "UObject/WeakInterfacePtr.h"

class IAudioPlayer;

namespace MLibrary
{
	namespace UE::Audio
	{
		class MLIBRARY_API AudioKit
		{
			private:
				AudioKit() = delete;
				~AudioKit() = delete;

			public:
				static void AssignAudioPlayer(IAudioPlayer*);
				static FSoundEffectHandle PlaySE3D(const FString&, float volume = 1.0f, const FVector& location = FVector::ZeroVector);
				static FSoundEffectHandle PlaySE2D(const FString&, float volume = 1.0f);
				static bool StopSE(const FSoundEffectHandle&);
				static void PlayBGM(const FString&, float volume = 1.0f);
				static bool StopBGM();

			private:
				static TWeakInterfacePtr<IAudioPlayer> m_audioPlayer;
		};
	}
}

#endif
