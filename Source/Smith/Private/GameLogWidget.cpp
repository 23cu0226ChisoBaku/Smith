// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogWidget.h"

#include "Debug.h"

void UGameLogWidget::NativeConstruct()
{
  Super::NativeConstruct();
  m_isVisibility = false;
  m_currentAlpha = GetColorAndOpacity();
}

void UGameLogWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);
  m_timer += InDeltaTime;

  if ((m_visibleTime <= m_timer) && (m_isVisibility))
  {
    // 非表示にする
    m_currentAlpha.A -= m_alphaSubtractionSpeed;
    if(m_currentAlpha.A <= 0.0f)
    {
      // リセット処理
      m_timer = 0.0f;
      m_currentAlpha.A = 0.0f;
      m_isVisibility = false;
      SetVisibility((ESlateVisibility)HIDDEN);
    }
    // カラー（アルファ）をセット
    SetColorAndOpacity(m_currentAlpha);
  }
}

void UGameLogWidget::AddLogMessage(FString message)
{
  m_logArray.Add(message);
}

void UGameLogWidget::OutPutLog()
{
  if (m_maxVisibleQuantity <= m_logArray.Num())
  {
    int32 difference = m_logArray.Num() - m_maxVisibleQuantity;
    // 表示個数より超えていたら、差分古い順から消す
    for (int32 i = 0; i < difference; ++i)
    {
      m_logArray.RemoveAt(0);
    }
  }

  FString log;

  for (auto str : m_logArray)
  {
    log.Append(str);
  }

  m_outPutLog = log;

  OnLog_BP();

  // 表示する
  SetVisibility((ESlateVisibility)VISIBLE);
  m_currentAlpha.A = 1.0f;
  SetColorAndOpacity(m_currentAlpha);
  m_isVisibility = true;
  m_timer = 0.0f;
}