// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenFade.h"
#include "Components/Border.h"
#include "MLibrary.h"

void UScreenFade::NativeConstruct()
{
  Super::NativeConstruct();
  m_userWidget = this;
  m_status = None; 
}

void UScreenFade::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry,InDeltaTime);

  // TODO
  if(!m_animFinish)
  {
   return;
  }

  if(!OnFadeInEndEvent.IsBound() || !OnFadeOutEndEvent.IsBound())
  {
    MDebug::LogError("Not Bind");
    return;
  }

  if(IsFinish())
  {    
    // TODO
    if(m_status == Out)
    {
      OnFadeOutEndEvent.Broadcast();
    }
    else
    {
      OnFadeInEndEvent.Broadcast();
    }

    m_status = None;
    m_animFinish = false;

  }
}

bool UScreenFade::IsFinish()
{
  return m_animFinish;
}

void UScreenFade::StartFade(FadeStatus status)
{
  SetVisibility(ESlateVisibility::HitTestInvisible);
  if(status == In)
  {
    if(OnFadeInStartEvent.IsBound())
    {
      FadeIn();
      OnFadeInStartEvent.Broadcast();
      m_status = In;
    }
  }
  else if(status == Out)
  {
    if(OnFadeOutStartEvent.IsBound())
    {
      FadeOut();
      OnFadeOutStartEvent.Broadcast();
      m_status = Out;
    }
  }
}