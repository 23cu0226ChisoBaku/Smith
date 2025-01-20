// Fill out your copyright notice in the Description page of Project Settings.

#include "TestUserWidget.h"

#include "Debug.h"

void UTestUserWidget::NativeConstruct()
{
  Super::NativeConstruct();
  m_isVisibility = false;
}

void UTestUserWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);
  m_timer += InDeltaTime;

  if (m_visibleTime <= m_timer && m_isVisibility)
  {
    MDebug::LogError("Reset");
    // 非表示にする
    SetVisibility((ESlateVisibility)HIDDEN);
    m_timer = 0.0f;
    m_isVisibility = false;
  }
}

void UTestUserWidget::SetLogMessage(FString message)
{
  m_logArray.Emplace(message);
}

void UTestUserWidget::OutPutLog()
{
  if (m_maxVisibleQuantity <= m_logArray.Num())
  {
    int32 difference = m_logArray.Num() - m_maxVisibleQuantity;
    // 表示個数より超えていたら、差分古い順から消す
    for(int32 i = 0; i < difference; ++i)
    {
      m_logArray.RemoveAt(0);
    }
  }

  FString log;

  for (auto str : m_logArray)
  {
    log.Append(str + "\n");
  }

  m_outPutLog = log;

  OnLog_BP();

  SetVisibility((ESlateVisibility)VISIBLE);

  m_isVisibility = true;
  m_timer = 0.0f;
}