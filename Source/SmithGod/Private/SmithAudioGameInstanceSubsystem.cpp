// Fill out your copyright notice in the Description page of Project Settings.


#include "SmithAudioGameInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "AudioKit.h"
#include "SmithSoundMetaData_Import.h"
#include "MLibrary.h"

void USmithAudioGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);
  m_bIsTickable = true;

  MLibrary::UE::Audio::AudioKit::AssignAudioPlayer(this);
}

void USmithAudioGameInstanceSubsystem::Deinitialize()
{
  m_seBuffer.Empty();
  m_bgmBuffer.Empty();
  m_seHandle.Empty();
  m_seIdentifier.Empty();
  if (m_bgmComp != nullptr)
  {
    m_bgmComp->Stop();
    m_bgmComp->DestroyComponent();
  }
  m_bIsTickable = false;
  MLibrary::UE::Audio::AudioKit::DetachAudioPlayer();

  Super::Deinitialize();
}

void USmithAudioGameInstanceSubsystem::InitializeSounds(UDataTable* bgmTable, UDataTable* seTable)
{
  TArray<FSmithSoundMetaData_Import*> audioSources;
  if (bgmTable != nullptr)
  {
    bgmTable->GetAllRows<FSmithSoundMetaData_Import>(nullptr, audioSources);
    for (const auto& audioSource : audioSources)
    {
      if (!m_bgmBuffer.Contains(audioSource->SoundName))
      {
        m_bgmBuffer.Emplace(audioSource->SoundName, audioSource->Sound);
      }
    }
  }

  audioSources.Reset();
  if (seTable != nullptr)
  {
    seTable->GetAllRows<FSmithSoundMetaData_Import>(nullptr, audioSources);
    for (const auto& audioSource : audioSources)
    {
      if (!m_seBuffer.Contains(audioSource->SoundName))
      {
        m_seIdentifier.Emplace(audioSource->SoundName, FGuid::NewGuid().ToString());
        m_seBuffer.Emplace(audioSource->SoundName, audioSource->Sound);
      }
    }   
  }
}

FSoundEffectHandle USmithAudioGameInstanceSubsystem::PlaySE3D(const FString& seName, float volume, const FVector& location)
{
  if (!m_seBuffer.Contains(seName))
  {
    return {};
  }

  USoundBase* se = m_seBuffer[seName].IsValid() ? m_seBuffer[seName].Get() : m_seBuffer[seName].LoadSynchronous();
  if (se != nullptr)
  {
    UAudioComponent* comp = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), se, location, FRotator::ZeroRotator, volume);
    if (comp != nullptr)
    {
      comp->Play();
      FSoundEffectHandle seHandle(m_seIdentifier[seName], comp);
      m_seHandle.Emplace(seHandle);
      return seHandle;
    }
  }

  return {};
}
FSoundEffectHandle USmithAudioGameInstanceSubsystem::PlaySE(const FString& seName, float volume)
{
  if (!m_seBuffer.Contains(seName))
  {
    return {};
  }

  USoundBase* se = m_seBuffer[seName].IsValid() ? m_seBuffer[seName].Get() : m_seBuffer[seName].LoadSynchronous();
  if (se != nullptr)
  {
    UAudioComponent* comp = UGameplayStatics::CreateSound2D(GetWorld(), se, volume);
    if (comp != nullptr)
    {
      comp->Play();
      FSoundEffectHandle seHandle(m_seIdentifier[seName], comp);
      m_seHandle.Emplace(seHandle);
      return seHandle;
    }
  }

  return {};
}
bool USmithAudioGameInstanceSubsystem::StopSE(const FSoundEffectHandle& seHandle)
{
  int32 idx = 0;

  while (idx < m_seHandle.Num())
  {
    const FSoundEffectHandle& handle = m_seHandle[idx];
    if (!handle.IsValid())
    {
      m_seHandle.Remove(handle);
      continue;
    }

    if (IsEqual(seHandle, handle))
    {
      handle->Stop();
      m_seHandle.Remove(handle);
      return true;
    }

    ++idx;
  }

  return false;
}

void USmithAudioGameInstanceSubsystem::PlayBGM(const FString& bgmName, float volume)
{
  if (!m_bgmBuffer.Contains(bgmName))
  {
    return;
  }

  USoundBase* se = m_bgmBuffer[bgmName].IsValid() ? m_bgmBuffer[bgmName].Get() : m_bgmBuffer[bgmName].LoadSynchronous();
  if (se != nullptr)
  {
    if (m_bgmComp == nullptr)
    {
      m_bgmComp = UGameplayStatics::CreateSound2D(GetWorld(), se, volume);
      if (m_bgmComp != nullptr)
      {
        m_bgmComp->Play();
      }
    }
    else
    {
      if (m_bgmComp->IsPlaying())
      {
        m_bgmComp->Stop();
      }

      m_bgmComp->SetSound(se);
      m_bgmComp->Play();
    }
  }
}
bool USmithAudioGameInstanceSubsystem::StopBGM()
{
  if (!::IsValid(m_bgmComp))
  {
    return false;
  }

  m_bgmComp->Stop();
  m_bgmComp->SetSound(nullptr);
  return true;
}

bool USmithAudioGameInstanceSubsystem::IsTickable() const
{
  return m_bIsTickable;
}

void USmithAudioGameInstanceSubsystem::Tick(float DeltaTime)
{
  if (!m_bIsTickable)
  {
    return;
  }

  int32 idx = 0;
  while (idx < m_seHandle.Num())
  {
    const FSoundEffectHandle& handle = m_seHandle[idx];
    if (!handle.IsValid())
    {
      m_seHandle.RemoveAt(idx);
      continue;
    }

    if (!handle->IsPlaying())
    {
      handle->Stop();
      handle->DestroyComponent();
      m_seHandle.RemoveAt(idx);
      continue;
    }

    ++idx;
  }
} 

TStatId USmithAudioGameInstanceSubsystem::GetStatId() const
{
  RETURN_QUICK_DECLARE_CYCLE_STAT(USmithAudioGameInstanceSubsystem, STATGROUP_Tickables);
}

